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
typedef typename std::vector<MultiArc*> MultiArcList;

template <typename AL>
bool removeArcFromList(AL &list, Arc *arc);
template <typename AL>
bool removeBundledArcFromList(AL &list, Arc *arc);
template <typename AL>
bool isArcInList(AL &list, Arc *arc);
template <typename AL>
bool isBundledArc(AL &list, Arc *arc);

class IncidenceListVertex::CheshireCat {
public:
    bool checkConsisteny;
    ArcList outgoingArcs;
    ArcList incomingArcs;
    MultiArcList outgoingMultiArcs;
    MultiArcList incomingMultiArcs;
};

IncidenceListVertex::IncidenceListVertex(GraphArtifact *parent)
    : Vertex(parent), grin(new CheshireCat)
{
    grin->checkConsisteny = true;
}

IncidenceListVertex::~IncidenceListVertex()
{
    delete grin;
}

int IncidenceListVertex::getOutDegree() const
{
    int deg = grin->outgoingArcs.size();
    for (MultiArc *ma : grin->outgoingMultiArcs) {
        deg += ma->getSize();
    }
    return deg;
}

void IncidenceListVertex::addOutgoingArc(Arc *a)
{
    if (grin->checkConsisteny && a->getTail() != this) {
        throw std::invalid_argument("Arc has other tail.");
    }
    MultiArc *ma = dynamic_cast<MultiArc*>(a);
    if (ma) {
        grin->outgoingMultiArcs.push_back(ma);
    } else {
        grin->outgoingArcs.push_back(a);
    }
}

void IncidenceListVertex::removeOutgoingArc(Arc *a)
{
    if (grin->checkConsisteny && a->getTail() != this) {
        throw std::invalid_argument("Arc has other tail.");
    }
    if (!removeArcFromList(grin->outgoingArcs, a)
            && !removeArcFromList(grin->outgoingMultiArcs, a)) {
        removeBundledArcFromList(grin->outgoingMultiArcs, a);
    }
}

void IncidenceListVertex::clearOutgoingArcs()
{
    grin->outgoingArcs.clear();
    grin->outgoingMultiArcs.clear();
}

int IncidenceListVertex::getInDegree() const
{
    int deg = grin->incomingArcs.size();
    for (MultiArc *ma : grin->incomingMultiArcs) {
        deg += ma->getSize();
    }
    return deg;
}

void IncidenceListVertex::addIncomingArc(Arc *a)
{
    if (grin->checkConsisteny && a->getHead() != this) {
        throw std::invalid_argument("Arc has other head.");
    }
    MultiArc *ma = dynamic_cast<MultiArc*>(a);
    if (ma) {
        grin->incomingMultiArcs.push_back(ma);
    } else {
        grin->incomingArcs.push_back(a);
    }
}

void IncidenceListVertex::removeIncomingArc(Arc *a)
{
    if (grin->checkConsisteny && a->getHead() != this) {
        throw std::invalid_argument("Arc has other head.");
    }
    if (!removeArcFromList(grin->incomingArcs, a)
            && !removeArcFromList(grin->incomingMultiArcs, a)) {
        removeBundledArcFromList(grin->incomingMultiArcs, a);
    }
}

void IncidenceListVertex::clearIncomingArcs()
{
    grin->incomingArcs.clear();
    grin->incomingMultiArcs.clear();
}

void IncidenceListVertex::enableConsistencyCheck(bool enable)
{
    grin->checkConsisteny = enable;
}

bool IncidenceListVertex::hasOutgoingArc(Arc *a) const
{
    return isArcInList(grin->outgoingArcs, a)
            || isArcInList(grin->outgoingMultiArcs, a)
            || isBundledArc(grin->outgoingMultiArcs, a);
}

bool IncidenceListVertex::hasIncomingArc(Arc *a) const
{
    return isArcInList(grin->incomingArcs, a)
            || isArcInList(grin->incomingMultiArcs, a)
            || isBundledArc(grin->incomingArcs, a);
}

void IncidenceListVertex::acceptOutgoingArcVisitor(ArcVisitor *aVisitor) const
{
    mapOutgoingArcs(aVisitor->getVisitorFunction(), arcFalse);
}

void IncidenceListVertex::acceptIncomingArcVisitor(ArcVisitor *aVisitor) const
{
    mapIncomingArcs(aVisitor->getVisitorFunction(), arcFalse);
}

bool IncidenceListVertex::mapOutgoingArcs(ArcMapping avFun, ArcPredicate breakCondition) const
{
    for (Arc *a : grin->outgoingArcs) {
        if (breakCondition(a)) {
            return false;
        }
        avFun(a);
    }
    for (Arc *a : grin->outgoingMultiArcs) {
        if (breakCondition(a)) {
            return false;
        }
        avFun(a);
    }
    return true;
}

bool IncidenceListVertex::mapIncomingArcs(ArcMapping avFun, ArcPredicate breakCondition) const
{
    for (Arc *a : grin->incomingArcs) {
        if (breakCondition(a)) {
            return false;
        }
        avFun(a);
    }
    for (Arc *a : grin->incomingMultiArcs) {
        if (breakCondition(a)) {
            return false;
        }
        avFun(a);
    }
    return true;
}

template <typename AL>
bool removeArcFromList(AL &list, Arc *arc) {
    auto it = std::find(list.cbegin(), list.cend(), arc);
    if (it != list.cend()) {
        list.erase(it);
        return true;
    }
    return false;
}

template <typename AL>
bool removeBundledArcFromList(AL &list, Arc *arc) {
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

template <typename AL>
bool isArcInList(AL &list, Arc *arc) {
    return std::find(list.cbegin(), list.cend(), arc) != list.cend();
}

template <typename AL>
bool isBundledArc(AL &list, Arc *arc) {
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
