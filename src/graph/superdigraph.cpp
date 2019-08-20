/**
 * Copyright (C) 2013 - 2019 : Kathrin Hanauer
 *
 * This file is part of Algora.
 *
 * Algora is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Algora is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Algora.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Contact information:
 *   http://algora.xaikal.org
 */

#include "superdigraph.h"
#include "graph.incidencelist/incidencelistgraphimplementation.h"
#include "graph.incidencelist/incidencelistvertex.h"

#include <unordered_map>

namespace Algora {

IncidenceListVertex *findVertex(Vertex *v,
                                std::unordered_map<const Vertex *, IncidenceListVertex *> &map,
                                const DiGraph *graph);
const IncidenceListVertex *findVertex(const Vertex *v,
                                std::unordered_map<const Vertex *, IncidenceListVertex *> &map,
                                const DiGraph *graph);
IncidenceListVertex *findOrCreateVertex(Vertex *v,
                                  std::unordered_map<const Vertex *, IncidenceListVertex *> &map,
                                  IncidenceListGraphImplementation *impl, SuperDiGraph *graph);

//class DummyVertex : public IncidenceListVertex {
//public:
//    explicit DummyVertex(SuperDiGraph *g) : IncidenceListVertex(0U, g) { enableConsistencyCheck(false); }
//    virtual ~DummyVertex() { }
//};

class SuperDiGraph::CheshireCat {
public:
    DiGraph *subGraph;
    IncidenceListGraphImplementation *extra;
    std::unordered_map<const Vertex*,IncidenceListVertex*> map;

    explicit CheshireCat(DiGraph *subGraph) : subGraph(subGraph) { }
    ~CheshireCat() { delete extra; }
};

SuperDiGraph::SuperDiGraph(DiGraph *graph)
    : grin(new CheshireCat(graph))
{
    grin->extra = new IncidenceListGraphImplementation(this);
    graph->onVertexAdd(this, [&](Vertex *v) {
        greetVertex(v);
    });
    graph->onVertexRemove(this, [&](Vertex *v) {
        dismissVertex(v);
        auto i = grin->map.find(v);
        if (i != grin->map.end()) {
            IncidenceListVertex *vertex = i->second;
            vertex->mapOutgoingArcs([&](Arc *a) { removeArc(a); });
            vertex->mapIncomingArcs([&](Arc *a) { removeArc(a); });
            grin->extra->removeVertex(vertex);
            grin->map.erase(i);
        }
    });
    graph->onArcAdd(this, [&](Arc *a) {
        greetArc(a);
    });
    graph->onArcRemove(this, [&](Arc *a) {
        dismissArc(a);
    });
}

SuperDiGraph::~SuperDiGraph()
{
    grin->extra->mapArcs([&](Arc *a) {
        removeArc(a);
    }, arcFalse);
    grin->subGraph->removeOnVertexAdd(this);
    grin->subGraph->removeOnVertexRemove(this);
    grin->subGraph->removeOnArcAdd(this);
    grin->subGraph->removeOnArcRemove(this);
    delete grin;
}

Vertex *SuperDiGraph::addVertex()
{
    auto v = grin->extra->recycleOrCreateIncidenceListVertex();
    grin->extra->addVertex(v);
    greetVertex(v);
    return v;
}

void SuperDiGraph::removeVertex(Vertex *v)
{
    if (v->getParent() == this) {
        auto vertex = dynamic_cast<IncidenceListVertex*>(v);
        if (!vertex) {
            throw std::invalid_argument("Vertex is not a part of this graph.");
        }
        dismissVertex(vertex);
        grin->extra->removeVertex(vertex);
    } else {
        grin->subGraph->removeVertex(v);
    }
}

bool SuperDiGraph::containsVertex(const Vertex *v) const
{
    if (v->getParent() != this) {
        return grin->subGraph->containsVertex(v);
    }
    auto vertex = dynamic_cast<const IncidenceListVertex*>(v);
    if (!vertex) {
        throw std::invalid_argument("Vertex is not a part of this graph.");
    }
    return grin->extra->containsVertex(vertex);
}

Vertex *SuperDiGraph::getAnyVertex() const
{
    if (!grin->extra->isEmpty()) {
        return grin->extra->getFirstVertex();
    } else {
        return grin->subGraph->getAnyVertex();
    }
}

void SuperDiGraph::mapVerticesUntil(const VertexMapping &vvFun, const VertexPredicate &breakCondition)
{
    grin->extra->mapVertices([&](Vertex *v) {
        //if (!dynamic_cast<DummyVertex*>(v)) {
        if (v->getParent()) {
            vvFun(v);
        }
    }, breakCondition);
    grin->subGraph->mapVerticesUntil(vvFun, breakCondition);
}

bool SuperDiGraph::isEmpty() const
{
    return grin->extra->isEmpty() && grin->subGraph->isEmpty();
}

Graph::size_type SuperDiGraph::getSize() const
{
    return grin->extra->getSize() + grin->subGraph->getSize() - grin->map.size();
}

void SuperDiGraph::clear()
{
    grin->extra->mapArcs([&](Arc *a) {
        removeArc(a);
    }, arcFalse);
    delete grin->extra;
    grin->extra = new IncidenceListGraphImplementation(this);
}

Arc *SuperDiGraph::addArc(Vertex *tail, Vertex *head)
{
    IncidenceListVertex *t = findOrCreateVertex(tail, grin->map, grin->extra, this);
    IncidenceListVertex *h = findOrCreateVertex(head, grin->map, grin->extra, this);
    // ugly hack
    t->enableConsistencyCheck(false);
    h->enableConsistencyCheck(false);

    Arc *a = createArc(tail, head);
    grin->extra->addArc(a, t, h);
    greetArc(a);
    return a;
}

MultiArc *SuperDiGraph::addMultiArc(Vertex *tail, Vertex *head, size_type size)
{
    if (size <= 0) {
        throw std::invalid_argument("Multiarcs must be of size at least 1.");
    }

    IncidenceListVertex *t = findOrCreateVertex(tail, grin->map, grin->extra, this);
    IncidenceListVertex *h = findOrCreateVertex(head, grin->map, grin->extra, this);
    // ugly hack
    t->enableConsistencyCheck(false);
    h->enableConsistencyCheck(false);

    MultiArc *a = createMultiArc(tail, head, size);
    grin->extra->addArc(a, t, h);
    greetArc(a);
    return a;
}

void SuperDiGraph::removeArc(Arc *a)
{
    if (a->getParent() != this) {
        grin->subGraph->removeArc(a);
        return;
    }
    auto t = findVertex(a->getTail(), grin->map, this);
    auto h = findVertex(a->getHead(), grin->map, this);

    if (!t || !h) {
        throw std::invalid_argument("Arc is not a part of this graph.");
    }
    dismissArc(a);

    grin->extra->removeArc(a, t, h);
}

bool SuperDiGraph::containsArc(const Arc *a) const
{
    if (a->getParent() != this) {
        return grin->subGraph->containsArc(a);
    }

    auto tail = findVertex(a->getTail(), grin->map, this);

    if (!tail) {
        return false;
    }
    return grin->extra->containsArc(a, tail);
}

Arc *SuperDiGraph::findArc(const Vertex *from, const Vertex *to) const
{
    if (from->getParent() != this && to->getParent() != this) {
        Arc *arc = grin->subGraph->findArc(from, to);
        if (arc != nullptr) {
            return arc;
        }
    }
    auto t = findVertex(from, grin->map, this);
    // the arc knows nothing about dummy vertices...
    auto h = dynamic_cast<const IncidenceListVertex*>(to);

    if (!t || !h) {
        return nullptr;
    }
    return grin->extra->findArc(t, h);
}

SuperDiGraph::size_type SuperDiGraph::getOutDegree(const Vertex *v, bool multiArcsAsSimple) const
{
    const IncidenceListVertex *vertex;
    if (v->getParent() == this) {
        vertex = dynamic_cast<const IncidenceListVertex*>(v);
        if (!vertex) {
            throw std::invalid_argument("Vertex is not a part of this graph.");
        }
        return grin->extra->getOutDegree(vertex, multiArcsAsSimple);
    }

    vertex = findVertex(v, grin->map, this);
    if (vertex) {
        return grin->extra->getOutDegree(vertex, multiArcsAsSimple) + grin->subGraph->getOutDegree(v, multiArcsAsSimple);
    }

    return grin->subGraph->getOutDegree(v, multiArcsAsSimple);
}

SuperDiGraph::size_type SuperDiGraph::getInDegree(const Vertex *v, bool multiArcsAsSimple) const
{
    const IncidenceListVertex *vertex;
    if (v->getParent() == this) {
        vertex = dynamic_cast<const IncidenceListVertex*>(v);
        if (!vertex) {
            throw std::invalid_argument("Vertex is not a part of this graph.");
        }
        return grin->extra->getInDegree(vertex, multiArcsAsSimple);
    }

    vertex = findVertex(v, grin->map, this);
    if (vertex) {
        return grin->extra->getInDegree(vertex, multiArcsAsSimple) + grin->subGraph->getInDegree(v, multiArcsAsSimple);
    }

    return grin->subGraph->getInDegree(v, multiArcsAsSimple);
}

void SuperDiGraph::mapArcsUntil(const ArcMapping &avFun, const ArcPredicate &breakCondition)
{
    grin->extra->mapArcs(avFun, breakCondition);
    grin->subGraph->mapArcsUntil(avFun, breakCondition);
}

void SuperDiGraph::mapOutgoingArcsUntil(const Vertex *v, const ArcMapping &avFun, const ArcPredicate &breakCondition)
{
    const IncidenceListVertex *vertex;
    if (v->getParent() == this) {
        vertex = dynamic_cast<const IncidenceListVertex*>(v);
        if (!vertex) {
            throw std::invalid_argument("Vertex is not a part of this graph.");
        }
        grin->extra->mapOutgoingArcs(vertex, avFun, breakCondition);
    } else {
        vertex = findVertex(v, grin->map, this);
        if (vertex) {
            grin->extra->mapOutgoingArcs(vertex, avFun, breakCondition);
        }
        grin->subGraph->mapOutgoingArcsUntil(v, avFun, breakCondition);
    }
}

void SuperDiGraph::mapIncomingArcsUntil(const Vertex *v, const ArcMapping &avFun, const ArcPredicate &breakCondition)
{
    const IncidenceListVertex *vertex;
    if (v->getParent() == this) {
        vertex = dynamic_cast<const IncidenceListVertex*>(v);
        if (!vertex) {
            throw std::invalid_argument("Vertex is not a part of this graph.");
        }
        grin->extra->mapIncomingArcs(vertex, avFun, breakCondition);
    } else {
        vertex = findVertex(v, grin->map, this);
        if (vertex) {
            grin->extra->mapIncomingArcs(vertex, avFun, breakCondition);
        }
        grin->subGraph->mapIncomingArcsUntil(v, avFun, breakCondition);
    }
}

SuperDiGraph::size_type SuperDiGraph::getNumArcs(bool multiArcsAsSimple) const
{
    return grin->extra->getNumArcs(multiArcsAsSimple) + grin->subGraph->getNumArcs(multiArcsAsSimple);
}

IncidenceListVertex *findVertex(Vertex *v, std::unordered_map<const Vertex *, IncidenceListVertex *> &map,
                                const DiGraph *graph) {
    if (v->getParent() == graph) {
        auto vertex = dynamic_cast<IncidenceListVertex*>(v);
        if (!vertex) {
            throw std::invalid_argument("Vertex is not a part of this graph.");
        }
        return vertex;
    }

    if (map.count(v) > 0) {
        return map.at(v);
    }

    return nullptr;
}

const IncidenceListVertex *findVertex(const Vertex *v, std::unordered_map<const Vertex *, IncidenceListVertex *> &map,
                                const DiGraph *graph) {
    if (v->getParent() == graph) {
        auto vertex = dynamic_cast<const IncidenceListVertex*>(v);
        if (!vertex) {
            throw std::invalid_argument("Vertex is not a part of this graph.");
        }
        return vertex;
    }

    if (map.count(v) > 0) {
        return map.at(v);
    }

    return nullptr;
}

IncidenceListVertex *findOrCreateVertex(Vertex *v,
                                  std::unordered_map<const Vertex*,IncidenceListVertex*> &map,
                                  IncidenceListGraphImplementation *impl, SuperDiGraph *graph) {
    IncidenceListVertex *vertex = findVertex(v, map, graph);
    if (vertex) {
        return vertex;
    }

    //vertex = new DummyVertex(graph);
    vertex = impl->createIncidenceListVertex();
    vertex->setParent(nullptr);
    impl->addVertex(vertex);
    map[v] = vertex;
    return vertex;
}

}
