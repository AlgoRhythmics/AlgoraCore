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

}

SubDiGraph::~SubDiGraph()
{

}

Vertex *SubDiGraph::addVertex()
{
    return superGraph->addVertex();
}

void SubDiGraph::removeVertex(Vertex *v)
{
    /*if (!containsVertex(v)) {
        throw std::invalid_argument("Vertex is not a part of this graph.");
    }*/
    superGraph->removeVertex(v);
}

bool SubDiGraph::containsVertex(Vertex *v) const
{
    return inSubGraph(v) && superGraph->containsVertex(v);
}

Vertex *SubDiGraph::getAnyVertex() const
{
    Vertex *vertex = 0;
    superGraph->visitVerticesUntil([&](Vertex *v) {
        if (vertex == 0 && inSubGraph(v)) {
            vertex = v;
        }
    }, [&](const Vertex *) {
        return vertex != 0;
    });
    return vertex;
}

void SubDiGraph::visitVerticesUntil(VertexVisitorFunc vvFun, VertexPredicate breakCondition)
{
    superGraph->visitVerticesUntil([&](Vertex *v) {
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
    superGraph->visitVertices([&](Vertex *v) {
        if (inSubGraph(v)) {
            empty = false;
        }
    });
    return empty;
}

int SubDiGraph::getSize() const
{
    int n = 0;
    SubDiGraph *me = const_cast<SubDiGraph*>(this);
    me->visitVertices([&n](Vertex *){ n++; });
    return n;
}

Arc *SubDiGraph::addArc(Vertex *tail, Vertex *head)
{
    return superGraph->addArc(tail, head);
}

void SubDiGraph::removeArc(Arc *a)
{
    superGraph->removeArc(a);
}

bool Algora::SubDiGraph::containsArc(Arc *a) const
{
    return inSubGraph(a)
            && inSubGraph(a->getTail())
            && inSubGraph(a->getHead())
            && superGraph->containsArc(a);
}

int SubDiGraph::getOutDegree(const Vertex *v) const
{
    int out = 0;
    SubDiGraph *me = const_cast<SubDiGraph*>(this);
    me->visitOutgoingArcs(v, [&out](Arc *) { out++;});
    return out;
}

int SubDiGraph::getInDegree(const Vertex *v) const
{
    int in = 0;
    SubDiGraph *me = const_cast<SubDiGraph*>(this);
    me->visitIncomingArcs(v, [&in](Arc *) { in++; });
    return in;
}

void SubDiGraph::visitArcsUntil(ArcVisitorFunc avFun, ArcPredicate breakCondition)
{
    superGraph->visitArcsUntil([&](Arc *a) {
        if (inSubGraph(a) && inSubGraph(a->getTail()) && inSubGraph(a->getHead())) {
            avFun(a);
        }
    }, breakCondition);
}

void SubDiGraph::visitOutgoingArcsUntil(const Vertex *v, ArcVisitorFunc avFun, ArcPredicate breakCondition)
{
    superGraph->visitOutgoingArcsUntil(v, [&](Arc *a) {
        if (inSubGraph(a) && inSubGraph(a->getTail()) && inSubGraph(a->getHead())) {
            avFun(a);
        }
    }, breakCondition);
}

void SubDiGraph::visitIncomingArcsUntil(const Vertex *v, ArcVisitorFunc avFun, ArcPredicate breakCondition)
{
    superGraph->visitIncomingArcsUntil(v, [&](Arc *a) {
        if (inSubGraph(a) && inSubGraph(a->getTail()) && inSubGraph(a->getHead())) {
            avFun(a);
        }
    }, breakCondition);
}

}
