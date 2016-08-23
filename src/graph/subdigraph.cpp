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


#include "subdigraph.h"

#include "vertex.h"
#include "arc.h"
#include "property/propertymap.h"

#include <stdexcept>

namespace Algora {


SubDiGraph::SubDiGraph(DiGraph *graph, Property<bool> &inherit)
    : superGraph(graph), inSubGraph(inherit)
{
    graph->onVertexAdd(this, [&](Vertex *v) {
        if (inSubGraph(v)) {
          greetVertex(v);
        }
    });
    graph->onVertexRemove(this, [&](Vertex *v) {
        if (inSubGraph(v)) {
            dismissVertex(v);
        }
    });
    graph->onArcAdd(this, [&](Arc *a) {
        if (inSubGraph(a) && inSubGraph(a->getTail()) && inSubGraph(a->getHead())) {
            greetArc(a);
        }
    });
    graph->onArcRemove(this, [&](Arc *a) {
        if (inSubGraph(a) && inSubGraph(a->getTail()) && inSubGraph(a->getHead())) {
            dismissArc(a);
        }
    });
}

SubDiGraph::~SubDiGraph()
{
    superGraph->removeOnVertexAdd(this);
    superGraph->removeOnVertexRemove(this);
    superGraph->removeOnArcAdd(this);
    superGraph->removeOnArcRemove(this);
}

Vertex *SubDiGraph::addVertex()
{
    Vertex *v = superGraph->addVertex();
    if (!inSubGraph(v)) {
        superGraph->removeVertex(v);
        throw std::logic_error("Added vertex will not part of this graph.");
    }
    return v;
}

void SubDiGraph::removeVertex(Vertex *v)
{
    if (!inSubGraph(v)) {
        throw std::invalid_argument("Vertex is not a part of this graph.");
    }
    superGraph->removeVertex(v);
}

bool SubDiGraph::containsVertex(Vertex *v) const
{
    return inSubGraph(v) && superGraph->containsVertex(v);
}

Vertex *SubDiGraph::getAnyVertex() const
{
    Vertex *vertex = 0;
    superGraph->mapVerticesUntil([&](Vertex *v) {
        if (vertex == 0 && inSubGraph(v)) {
            vertex = v;
        }
    }, [&](const Vertex *) {
        return vertex != 0;
    });
    return vertex;
}

void SubDiGraph::mapVerticesUntil(VertexMapping vvFun, VertexPredicate breakCondition)
{
    superGraph->mapVerticesUntil([&](Vertex *v) {
        if (inSubGraph(v)) {
            vvFun(v);
        }
    }, breakCondition);
}

bool Algora::SubDiGraph::isEmpty() const
{
    if (superGraph->isEmpty()) {
        return true;
    }
    bool empty = true;
    superGraph->mapVertices([&](Vertex *v) {
        if (inSubGraph(v)) {
            empty = false;
        }
    });
    return empty;
}

Graph::size_type SubDiGraph::getSize() const
{
    size_type n = 0;
    SubDiGraph *me = const_cast<SubDiGraph*>(this);
    me->mapVertices([&n](Vertex *){ n++; });
    return n;
}

Arc *SubDiGraph::addArc(Vertex *tail, Vertex *head)
{
    if (!inSubGraph(tail) || !inSubGraph(head)) {
        throw std::invalid_argument("Vertex is not a part of this graph.");
    }
    Arc *a = superGraph->addArc(tail, head);
    if (!inSubGraph(a)) {
        superGraph->removeArc(a);
        throw std::logic_error("Added arc will not part of this graph.");
    }
    return a;
}

MultiArc *SubDiGraph::addMultiArc(Vertex *tail, Vertex *head, int size)
{
    if (size <= 0) {
        throw std::invalid_argument("Multiarcs must be of size at least 1.");
    }

    if (!inSubGraph(tail) || !inSubGraph(head)) {
        throw std::invalid_argument("Vertex is not a part of this graph.");
    }
    MultiArc *a = superGraph->addMultiArc(tail, head, size);
    if (!inSubGraph(a)) {
        superGraph->removeArc(a);
        throw std::logic_error("Added arc will not part of this graph.");
    }
    return a;
}

void SubDiGraph::removeArc(Arc *a)
{
    if (! (inSubGraph(a)
                && inSubGraph(a->getTail())
                && inSubGraph(a->getHead()))) {
        throw std::invalid_argument("Arc is not a part of this graph.");
    }
    superGraph->removeArc(a);
}

bool Algora::SubDiGraph::containsArc(Arc *a) const
{
    return inSubGraph(a)
            && inSubGraph(a->getTail())
            && inSubGraph(a->getHead())
            && superGraph->containsArc(a);
}

int SubDiGraph::getOutDegree(const Vertex *v, bool multiArcsAsSimple) const
{
    if (!inSubGraph(v)) {
        throw std::invalid_argument("Vertex is not a part of this graph.");
    }
    int out = 0;
    SubDiGraph *me = const_cast<SubDiGraph*>(this);
    me->mapOutgoingArcs(v, [&](Arc *a) { out += multiArcsAsSimple ? 1 : a->getSize();});
    return out;
}

int SubDiGraph::getInDegree(const Vertex *v, bool multiArcsAsSimple) const
{
    if (!inSubGraph(v)) {
        throw std::invalid_argument("Vertex is not a part of this graph.");
    }
    int in = 0;
    SubDiGraph *me = const_cast<SubDiGraph*>(this);
    me->mapIncomingArcs(v, [&](Arc *a) { in += multiArcsAsSimple ? 1 : a->getSize(); });
    return in;
}

void SubDiGraph::mapArcsUntil(ArcMapping avFun, ArcPredicate breakCondition)
{
    superGraph->mapArcsUntil([&](Arc *a) {
        if (inSubGraph(a) && inSubGraph(a->getTail()) && inSubGraph(a->getHead())) {
            avFun(a);
        }
    }, breakCondition);
}

void SubDiGraph::mapOutgoingArcsUntil(const Vertex *v, ArcMapping avFun, ArcPredicate breakCondition)
{
    if (!inSubGraph(v)) {
        throw std::invalid_argument("Vertex is not a part of this graph.");
    }
    superGraph->mapOutgoingArcsUntil(v, [&](Arc *a) {
        if (inSubGraph(a) && inSubGraph(a->getTail()) && inSubGraph(a->getHead())) {
            avFun(a);
        }
    }, breakCondition);
}

void SubDiGraph::mapIncomingArcsUntil(const Vertex *v, ArcMapping avFun, ArcPredicate breakCondition)
{
    if (!inSubGraph(v)) {
        throw std::invalid_argument("Vertex is not a part of this graph.");
    }
    superGraph->mapIncomingArcsUntil(v, [&](Arc *a) {
        if (inSubGraph(a) && inSubGraph(a->getTail()) && inSubGraph(a->getHead())) {
            avFun(a);
        }
    }, breakCondition);
}

DiGraph *SubDiGraph::createReversedGraph(PropertyMap<GraphArtifact *> &map) const
{
    DiGraph *superRev = superGraph->createReversedGraph(map);
    std::vector<Vertex*> rmVertices;
    superRev->mapVertices([&](Vertex *v) {
        if (!inSubGraph(map(v))) {
            rmVertices.push_back(v);
        }
    });
    for (Vertex *v : rmVertices) {
        superRev->removeVertex(v);
        map.resetToDefault(map(v));
        map.resetToDefault(v);
    }
    std::vector<Arc*> rmArcs;
    superRev->mapArcs([&](Arc *a) {
        if (!inSubGraph(map(a))) {
            rmArcs.push_back(a);
        }
    });
    for (Arc *a : rmArcs) {
        superRev->removeArc(a);
        map.resetToDefault(map(a));
        map.resetToDefault(a);
    }
    return superRev;
}

}
