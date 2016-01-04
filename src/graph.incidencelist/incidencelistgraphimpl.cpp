/**
 * Copyright (C) 2013 - 2015 : Kathrin Hanauer
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
#include "graph.visitor/vertexvisitor.h"
#include "graph.visitor/arcvisitor.h"

#include <vector>
#include <algorithm>

namespace Algora {

typedef typename std::vector<IncidenceListVertex*> VertexList;

class IncidenceListGraph::CheshireCat {
    VertexList vertices;

public:
    explicit CheshireCat();
    ~CheshireCat();

    void addVertex(IncidenceListVertex *vertex);
    void removeVertex(IncidenceListVertex *v);
    void addArc(Arc *a, IncidenceListVertex *tail, IncidenceListVertex *head);
    void removeArc(Arc *a, IncidenceListVertex *tail, IncidenceListVertex *head);
    void acceptVertexVisitor(VertexVisitor *nVisitor);
    void acceptArcVisitor(ArcVisitor *aVisitor);
    void acceptOutgoingArcVisitor(const IncidenceListVertex *v, ArcVisitor *aVisitor);
    void acceptIncomingArcVisitor(const IncidenceListVertex *v, ArcVisitor *aVisitor);
    bool isEmpty() const;
    int getSize() const;
};

IncidenceListGraph::CheshireCat::CheshireCat()
{

}

IncidenceListGraph::CheshireCat::~CheshireCat()
{
    for (IncidenceListVertex *v : vertices) {
        v->clearOutgoingArcs();
        v->clearIncomingArcs();
        delete v;
    }
    vertices.clear();
}

void IncidenceListGraph::CheshireCat::addVertex(IncidenceListVertex *vertex)
{
    vertices.push_back(vertex);
}

void IncidenceListGraph::CheshireCat::removeVertex(IncidenceListVertex *v)
{
    v->clearOutgoingArcs();
    v->clearIncomingArcs();
    vertices.erase(std::find(vertices.cbegin(), vertices.cend(), v));
}

void IncidenceListGraph::CheshireCat::addArc(Arc *a, IncidenceListVertex *tail, IncidenceListVertex *head)
{
    auto arc = std::shared_ptr<Arc>(a);
    tail->addOutgoingArc(arc);
    head->addIncomingArc(arc);
}

void IncidenceListGraph::CheshireCat::removeArc(Arc *a, IncidenceListVertex *tail, IncidenceListVertex *head)
{
    tail->removeOutgoingArc(a);
    head->removeIncomingArc(a);
}

void IncidenceListGraph::CheshireCat::acceptVertexVisitor(VertexVisitor *nVisitor)
{
    for (Vertex *v : vertices) {
        nVisitor->visitVertex(v);
    }
}

void IncidenceListGraph::CheshireCat::acceptArcVisitor(ArcVisitor *aVisitor)
{
    for (IncidenceListVertex *v : vertices) {
        v->acceptOutgoingArcVisitor(aVisitor);
    }
}

void IncidenceListGraph::CheshireCat::acceptOutgoingArcVisitor(const IncidenceListVertex *v, ArcVisitor *aVisitor)
{
    v->acceptOutgoingArcVisitor(aVisitor);
}

void IncidenceListGraph::CheshireCat::acceptIncomingArcVisitor(const IncidenceListVertex *v, ArcVisitor *aVisitor)
{
    v->acceptIncomingArcVisitor(aVisitor);
}

bool IncidenceListGraph::CheshireCat::isEmpty() const
{
    return vertices.empty();
}

int IncidenceListGraph::CheshireCat::getSize() const
{
    return vertices.size();
}

}
