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


#include "incidencelistvertex.h"

#include "incidencelistgraph.h"
#include "graph/arc.h"
#include "graph.visitor/arcvisitor.h"

#include <vector>
#include <stdexcept>
#include <algorithm>

namespace Algora {

typedef typename std::vector<std::shared_ptr<Arc> > ArcList;

void removeArcFromList(ArcList &list, Arc *arc);

class IncidenceListVertex::CheshireCat {
public:
    ArcList outgoingArcs;
    ArcList incomingArcs;
};

IncidenceListVertex::IncidenceListVertex(IncidenceListGraph *graph)
    : Vertex(graph), grin(new CheshireCat)
{

}

IncidenceListVertex::~IncidenceListVertex()
{
    delete grin;
}

int IncidenceListVertex::getOutDegree() const
{
    return grin->outgoingArcs.size();
}

void IncidenceListVertex::addOutgoingArc(std::shared_ptr<Arc> a)
{
    if (a->getTail() != this) {
        throw std::invalid_argument("Arc has other tail.");
    }
    grin->outgoingArcs.push_back(a);
}

void IncidenceListVertex::removeOutgoingArc(Arc *a)
{
    if (a->getTail() != this) {
        throw std::invalid_argument("Arc has other tail.");
    }
    removeArcFromList(grin->outgoingArcs, a);
}

void IncidenceListVertex::clearOutgoingArcs()
{
    grin->outgoingArcs.clear();
}

int IncidenceListVertex::getInDegree() const
{
    return grin->incomingArcs.size();
}

void IncidenceListVertex::addIncomingArc(std::shared_ptr<Arc> a)
{
    if (a->getHead() != this) {
        throw std::invalid_argument("Arc has other head.");
    }
    grin->incomingArcs.push_back(a);
}

void IncidenceListVertex::removeIncomingArc(Arc *a)
{
    if (a->getHead() != this) {
        throw std::invalid_argument("Arc has other head.");
    }
    removeArcFromList(grin->incomingArcs, a);
}

void IncidenceListVertex::clearIncomingArcs()
{
    grin->incomingArcs.clear();
}

void IncidenceListVertex::acceptOutgoingArcVisitor(ArcVisitor *aVisitor) const
{
    for (std::shared_ptr<Arc> a : grin->outgoingArcs) {
        aVisitor->visitArc(a.get());
    }
}

void IncidenceListVertex::acceptIncomingArcVisitor(ArcVisitor *aVisitor) const
{
    for (std::shared_ptr<Arc> a : grin->incomingArcs) {
        aVisitor->visitArc(a.get());
    }
}

void removeArcFromList(ArcList &list, Arc *arc) {
    list.erase(std::find_if(list.cbegin(), list.cend(),
                            [=](const std::shared_ptr<Arc> &a) { return a.get() == arc; }));
}

}
