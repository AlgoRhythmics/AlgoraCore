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


#include "incidencelistgraph.h"
#include "incidencelistvertex.h"
#include "graph/arc.h"
#include "graph/parallelarcsbundle.h"

#include "incidencelistgraphimpl.h"

#include <stdexcept>

namespace Algora {

IncidenceListVertex *castVertex(Vertex *v, IncidenceListGraph *graph);

IncidenceListGraph::IncidenceListGraph(GraphArtifact *parent)
    : DiGraph(parent), grin(new CheshireCat)
{

}

IncidenceListGraph::~IncidenceListGraph()
{
    delete grin;
}

Vertex *IncidenceListGraph::addVertex()
{
    auto v = createIncidenceListVertex();
    grin->addVertex(v);
    return v;
}

void IncidenceListGraph::removeVertex(Vertex *v)
{
    auto vertex = castVertex(v, this);

    grin->removeVertex(vertex);
}

bool IncidenceListGraph::containsVertex(Vertex *v) const
{
    if (v->getParent() != this) {
        return false;
    }

    auto vertex = dynamic_cast<IncidenceListVertex*>(v);

    if (!vertex) {
        return false;
    }

    return grin->containsVertex(vertex);
}

Vertex *IncidenceListGraph::getAnyVertex() const
{
    return grin->getFirstVertex();
}

void IncidenceListGraph::visitVertices(VertexVisitorFunc vvFun)
{
    grin->visitVertices(vvFun);
}

Arc *IncidenceListGraph::addArc(Vertex *tail, Vertex *head)
{
    auto t = castVertex(tail, this);
    auto h = castVertex(head, this);

    Arc *a = createArc(t, h);

    grin->addArc(a, t, h);
    return a;
}

void IncidenceListGraph::removeArc(Arc *a)
{
    if (a->getParent() != this) {
    //if (!containsArc(a)) {
        throw std::invalid_argument("Arc is not a part of this graph.");
    }

    auto tail = castVertex(a->getTail(), this);
    auto head = castVertex(a->getHead(), this);

    grin->removeArc(a, tail, head);
}

bool IncidenceListGraph::containsArc(Arc *a) const
{
    if (a->getParent() != this) {
        return false;
    }

    auto tail = dynamic_cast<IncidenceListVertex*>(a->getTail());
    if (!tail) {
        return false;
    }

    return grin->containsArc(a, tail);
}

void IncidenceListGraph::visitArcs(ArcVisitorFunc avFun)
{
    grin->visitArcs(avFun);
}

void IncidenceListGraph::visitOutgoingArcs(const Vertex *v, ArcVisitorFunc avFun)
{
    auto vertex = castVertex(const_cast<Vertex*>(v), this);
    grin->visitOutgoingArcs(vertex, avFun);
}

void IncidenceListGraph::visitIncomingArcs(const Vertex *v, ArcVisitorFunc avFun)
{
    auto vertex = castVertex(const_cast<Vertex*>(v), this);
    grin->visitIncomingArcs(vertex, avFun);
}

bool IncidenceListGraph::isEmpty() const
{
    return grin->isEmpty();
}

int IncidenceListGraph::getSize() const
{
    return grin->getSize();
}

void IncidenceListGraph::bundleParallelArcs()
{
    grin->bundleParallelArcs();
}

IncidenceListVertex *IncidenceListGraph::createIncidenceListVertex()
{
    return new IncidenceListVertex(this);
}

IncidenceListVertex *castVertex(Vertex *v, IncidenceListGraph *graph) {
    auto vertex = dynamic_cast<IncidenceListVertex*>(v);

    if (!vertex || vertex->getParent() != graph) {
        throw std::invalid_argument("Vertex is not a part of this graph.");
    }
    return vertex;
}

}
