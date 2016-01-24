/**
 * Copyright (C) 2016 : Kathrin Hanauer, Olaf Leßenich
 *
 * This file is part of Taffy.
 *
 * Taffy is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Taffy is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Taffy. If not, see <http://www.gnu.org/licenses/>.
 *
 * Contact information:
 *   https://github.com/xaikal/taffy
 *   taffy@xaikal.org
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
}

SuperDiGraph::~SuperDiGraph()
{
    grin->extra->visitArcs([&](Arc *a) {
        removeArc(a);
    }, arcFalse);
    delete grin;
}

Vertex *SuperDiGraph::addVertex()
{
    auto v = grin->extra->createIncidenceListVertex();
    grin->extra->addVertex(v);
    return v;
}

void SuperDiGraph::removeVertex(Vertex *v)
{
    if (v->getParent() == this) {
        auto vertex = dynamic_cast<IncidenceListVertex*>(v);
        if (!vertex) {
            throw std::invalid_argument("Vertex is not a part of this graph.");
        }
        grin->extra->removeVertex(vertex);
    } else {
        grin->subGraph->removeVertex(v);
        auto i = grin->map.find(v);
        if (i != grin->map.end()) {
            grin->extra->removeVertex(i->second);
            grin->map.erase(i);
        }
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

void SuperDiGraph::visitVerticesUntil(VertexVisitorFunc vvFun, VertexPredicate breakCondition)
{
    grin->extra->visitVertices([&](Vertex *v) {
        if (!dynamic_cast<DummyVertex*>(v)) {
            vvFun(v);
        }
    }, breakCondition);
    grin->subGraph->visitVerticesUntil(vvFun, breakCondition);
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

int SuperDiGraph::getOutDegree(const Vertex *v) const
{
    const IncidenceListVertex *vertex;
    if (v->getParent() == this) {
        vertex = dynamic_cast<const IncidenceListVertex*>(v);
        if (!vertex) {
            throw std::invalid_argument("Vertex is not a part of this graph.");
        }
        return grin->extra->getOutDegree(vertex);
    }

    vertex = findVertex(v, grin->map, this);
    if (vertex) {
        return grin->extra->getOutDegree(vertex) + grin->subGraph->getOutDegree(v);
    }

    return grin->subGraph->getOutDegree(v);
}

int SuperDiGraph::getInDegree(const Vertex *v) const
{
    const IncidenceListVertex *vertex;
    if (v->getParent() == this) {
        vertex = dynamic_cast<const IncidenceListVertex*>(v);
        if (!vertex) {
            throw std::invalid_argument("Vertex is not a part of this graph.");
        }
        return grin->extra->getInDegree(vertex);
    }

    vertex = findVertex(v, grin->map, this);
    if (vertex) {
        return grin->extra->getInDegree(vertex) + grin->subGraph->getInDegree(v);
    }

    return grin->subGraph->getInDegree(v);
}

void SuperDiGraph::visitArcsUntil(ArcVisitorFunc avFun, ArcPredicate breakCondition)
{
    grin->extra->visitArcs(avFun, breakCondition);
    grin->subGraph->visitArcsUntil(avFun, breakCondition);
}

void SuperDiGraph::visitOutgoingArcsUntil(const Vertex *v, ArcVisitorFunc avFun, ArcPredicate breakCondition)
{
    const IncidenceListVertex *vertex;
    if (v->getParent() == this) {
        vertex = dynamic_cast<const IncidenceListVertex*>(v);
        if (!vertex) {
            throw std::invalid_argument("Vertex is not a part of this graph.");
        }
        grin->extra->visitOutgoingArcs(vertex, avFun, breakCondition);
    } else {
        vertex = findVertex(v, grin->map, this);
        if (vertex) {
            grin->extra->visitOutgoingArcs(vertex, avFun, breakCondition);
        }
        grin->subGraph->visitOutgoingArcsUntil(v, avFun, breakCondition);
    }
}

void SuperDiGraph::visitIncomingArcsUntil(const Vertex *v, ArcVisitorFunc avFun, ArcPredicate breakCondition)
{
    const IncidenceListVertex *vertex;
    if (v->getParent() == this) {
        vertex = dynamic_cast<const IncidenceListVertex*>(v);
        if (!vertex) {
            throw std::invalid_argument("Vertex is not a part of this graph.");
        }
        grin->extra->visitIncomingArcs(vertex, avFun, breakCondition);
    } else {
        vertex = findVertex(v, grin->map, this);
        if (vertex) {
            grin->extra->visitIncomingArcs(vertex, avFun, breakCondition);
        }
        grin->subGraph->visitIncomingArcsUntil(v, avFun, breakCondition);
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
