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


#include "incidencelistvertex.h"

#include "incidencelistgraph.h"
#include "graph/arc.h"
#include "graph/parallelarcsbundle.h"
#include "graph.visitor/arcvisitor.h"

#include <vector>
#include <stdexcept>
#include <algorithm>

namespace Algora {

typedef typename std::vector<Arc*> ArcList;

bool removeArcFromList(ArcList &list, Arc *arc);
bool removeBundledArcFromList(ArcList &list, Arc *arc);
bool isArcInList(ArcList &list, Arc *arc);
bool isBundledArc(ArcList &list, Arc *arc);

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

void IncidenceListVertex::addOutgoingArc(Arc *a)
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
    if (!removeArcFromList(grin->outgoingArcs, a)) {
        removeBundledArcFromList(grin->outgoingArcs, a);
    }
}

void IncidenceListVertex::clearOutgoingArcs()
{
    grin->outgoingArcs.clear();
}

int IncidenceListVertex::getInDegree() const
{
    return grin->incomingArcs.size();
}

void IncidenceListVertex::addIncomingArc(Arc *a)
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
    if (!removeArcFromList(grin->incomingArcs, a)) {
        removeBundledArcFromList(grin->incomingArcs, a);
    }
}

void IncidenceListVertex::clearIncomingArcs()
{
    grin->incomingArcs.clear();
}

bool IncidenceListVertex::hasOutgoingArc(Arc *a) const
{
    return isArcInList(grin->outgoingArcs, a) || isBundledArc(grin->outgoingArcs, a);
}

bool IncidenceListVertex::hasIncomingArc(Arc *a) const
{
    return isArcInList(grin->incomingArcs, a) || isBundledArc(grin->incomingArcs, a);
}

void IncidenceListVertex::acceptOutgoingArcVisitor(ArcVisitor *aVisitor) const
{
    visitOutgoingArcs(aVisitor->getVisitorFunction());
}

void IncidenceListVertex::acceptIncomingArcVisitor(ArcVisitor *aVisitor) const
{
    visitIncomingArcs(aVisitor->getVisitorFunction());
}

void IncidenceListVertex::visitOutgoingArcs(ArcVisitorFunc avFun) const
{
    for (Arc *a : grin->outgoingArcs) {
        avFun(a);
    }
}

void IncidenceListVertex::visitIncomingArcs(ArcVisitorFunc avFun) const
{
    for (Arc *a : grin->incomingArcs) {
        avFun(a);
    }
}

bool removeArcFromList(ArcList &list, Arc *arc) {
    auto it = std::find(list.cbegin(), list.cend(), arc);
    if (it != list.cend()) {
        list.erase(it);
        return true;
    }
    return false;
}

bool removeBundledArcFromList(ArcList &list, Arc *arc) {
    for (Arc *a : list) {
        if (a->getHead() == arc->getHead() && a->getTail() == arc->getTail()) {
            ParallelArcsBundle *pab = dynamic_cast<ParallelArcsBundle*>(a);
            if (pab && pab->containsArc(arc)) {
                pab->removeArc(arc);
                return true;
            }
        }
    }
    return false;
}

bool isArcInList(ArcList &list, Arc *arc) {
    return std::find(list.cbegin(), list.cend(), arc) != list.cend();
}

bool isBundledArc(ArcList &list, Arc *arc) {
    for (Arc *a : list) {
        if (a->getHead() == arc->getHead() && a->getTail() == arc->getTail()) {
            ParallelArcsBundle *pab = dynamic_cast<ParallelArcsBundle*>(a);
            if (pab && pab->containsArc(arc)) {
                return true;
            }
        }
    }
    return false;
}

}
