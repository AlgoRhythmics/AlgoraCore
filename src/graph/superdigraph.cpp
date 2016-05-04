/**
 * Copyright (C) 2013 - 2016 : Kathrin Hanauer
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

class DummyVertex : public IncidenceListVertex {
public:
    DummyVertex(SuperDiGraph *g) : IncidenceListVertex(g) { enableConsistencyCheck(false); }
    virtual ~DummyVertex() { }
};

class SuperDiGraph::CheshireCat {
public:
    DiGraph *subGraph;
    IncidenceListGraphImplementation *extra;
    std::unordered_map<const Vertex*,IncidenceListVertex*> map;

    CheshireCat(DiGraph *subGraph) : subGraph(subGraph) { }
    ~CheshireCat() { delete extra; }
};

SuperDiGraph::SuperDiGraph(DiGraph *graph)
    : grin(new CheshireCat(graph))
{
    grin->extra = new IncidenceListGraphImplementation(this);
    graph->onVertexAdd([&](Vertex *v) {
        greetVertex(v);
    });
    graph->onVertexRemove([&](Vertex *v) {
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
    graph->onArcAdd([&](Arc *a) {
        greetArc(a);
    });
    graph->onArcRemove([&](Arc *a) {
        dismissArc(a);
    });
}

SuperDiGraph::~SuperDiGraph()
{
    grin->extra->mapArcs([&](Arc *a) {
        removeArc(a);
    }, arcFalse);
    delete grin;
}

Vertex *SuperDiGraph::addVertex()
{
    auto v = grin->extra->createIncidenceListVertex();
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

bool SuperDiGraph::containsVertex(Vertex *v) const
{
    if (v->getParent() != this) {
        return grin->subGraph->containsVertex(v);
    }
    auto vertex = dynamic_cast<IncidenceListVertex*>(v);
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

void SuperDiGraph::mapVerticesUntil(VertexMapping vvFun, VertexPredicate breakCondition)
{
    grin->extra->mapVertices([&](Vertex *v) {
        if (!dynamic_cast<DummyVertex*>(v)) {
            vvFun(v);
        }
    }, breakCondition);
    grin->subGraph->mapVerticesUntil(vvFun, breakCondition);
}

bool SuperDiGraph::isEmpty() const
{
    return grin->extra->isEmpty() && grin->subGraph->isEmpty();
}

int SuperDiGraph::getSize() const
{
    return grin->extra->getSize() + grin->subGraph->getSize() - grin->map.size();
}

Arc *SuperDiGraph::addArc(Vertex *tail, Vertex *head)
{
    IncidenceListVertex *t = findOrCreateVertex(tail, grin->map, grin->extra, this);
    IncidenceListVertex *h = findOrCreateVertex(head, grin->map, grin->extra, this);

    Arc *a = createArc(tail, head);
    grin->extra->addArc(a, t, h);
    greetArc(a);
    return a;
}

MultiArc *SuperDiGraph::addMultiArc(Vertex *tail, Vertex *head, int size)
{
    if (size <= 0) {
        throw std::invalid_argument("Multiarcs must be of size at least 1.");
    }

    IncidenceListVertex *t = findOrCreateVertex(tail, grin->map, grin->extra, this);
    IncidenceListVertex *h = findOrCreateVertex(head, grin->map, grin->extra, this);

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

bool SuperDiGraph::containsArc(Arc *a) const
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

int SuperDiGraph::getOutDegree(const Vertex *v, bool multiArcsAsSimple) const
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

    return grin->subGraph->getOutDegree(v);
}

int SuperDiGraph::getInDegree(const Vertex *v, bool multiArcsAsSimple) const
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

    return grin->subGraph->getInDegree(v);
}

void SuperDiGraph::mapArcsUntil(ArcMapping avFun, ArcPredicate breakCondition)
{
    grin->extra->mapArcs(avFun, breakCondition);
    grin->subGraph->mapArcsUntil(avFun, breakCondition);
}

void SuperDiGraph::mapOutgoingArcsUntil(const Vertex *v, ArcMapping avFun, ArcPredicate breakCondition)
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

void SuperDiGraph::mapIncomingArcsUntil(const Vertex *v, ArcMapping avFun, ArcPredicate breakCondition)
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

    return 0;
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

    return 0;
}

IncidenceListVertex *findOrCreateVertex(Vertex *v,
                                  std::unordered_map<const Vertex*,IncidenceListVertex*> &map,
                                  IncidenceListGraphImplementation *impl, SuperDiGraph *graph) {
    IncidenceListVertex *vertex = findVertex(v, map, graph);
    if (vertex) {
        return vertex;
    }

    vertex = new DummyVertex(graph);
    impl->addVertex(vertex);
    map[v] = vertex;
    return vertex;
}

}
