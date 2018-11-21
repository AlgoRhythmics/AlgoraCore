/**
 * Copyright (C) 2013 - 2018 : Kathrin Hanauer
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
#include "property/propertymap.h"

#include <vector>
#include <stdexcept>
#include <algorithm>
#include <cassert>

namespace Algora {

typedef typename std::vector<Arc*> ArcList;
typedef typename std::vector<MultiArc*> MultiArcList;

template <typename AL>
bool removeArcFromList(AL &list, PropertyMap<int> &indexMap, const Arc *arc);
bool removeBundledArcFromList(PropertyMap<ParallelArcsBundle*> &bundleMap, const Arc *arc);
template <typename AL>
bool isArcInList(const PropertyMap<int> &indexMap, AL &list, const Arc *arc);
template <typename AL>
bool isBundledArc(const PropertyMap<ParallelArcsBundle*> bundleMap, AL &list, const PropertyMap<int> &indexMap, const Arc *arc);

class IncidenceListVertex::CheshireCat {
public:
    int index;
    bool checkConsisteny;

    ArcList outgoingArcs;
    ArcList incomingArcs;
    MultiArcList outgoingMultiArcs;
    MultiArcList incomingMultiArcs;

    PropertyMap<ParallelArcsBundle*> bundle;

    PropertyMap<int> outIndex;
    PropertyMap<int> inIndex;
    PropertyMap<int> multiOutIndex;
    PropertyMap<int> multiInIndex;

    CheshireCat(int i) : index(i) {
        bundle.setDefaultValue(nullptr);
        outIndex.setDefaultValue(-1);
        inIndex.setDefaultValue(-1);
        multiOutIndex.setDefaultValue(-1);
        multiInIndex.setDefaultValue(-1);
    }
};

IncidenceListVertex::IncidenceListVertex(unsigned int id, GraphArtifact *parent, int index)
    : Vertex(id, parent), grin(new CheshireCat(index))
{
    grin->checkConsisteny = true;
}

IncidenceListVertex::~IncidenceListVertex()
{
    delete grin;
}

unsigned int IncidenceListVertex::getOutDegree(bool multiArcsAsSimple) const
{
    unsigned int deg = grin->outgoingArcs.size();
    if (multiArcsAsSimple) {
        return deg + grin->outgoingMultiArcs.size();
    }
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
        grin->multiOutIndex[ma] = grin->outgoingMultiArcs.size();
        grin->outgoingMultiArcs.push_back(ma);
        ParallelArcsBundle *pab = dynamic_cast<ParallelArcsBundle*>(ma);
        if (pab) {
            pab->mapArcs([&](Arc *a) {
                grin->bundle[a] = pab;
            });
        }
    } else {
        grin->outIndex[a] = grin->outgoingArcs.size();
        grin->outgoingArcs.push_back(a);
    }
}

void IncidenceListVertex::removeOutgoingArc(const Arc *a)
{
    if (grin->checkConsisteny && a->getTail() != this) {
        throw std::invalid_argument("Arc has other tail.");
    }
    if (!removeArcFromList(grin->outgoingArcs, grin->outIndex, a)
            && !removeArcFromList(grin->outgoingMultiArcs, grin->multiOutIndex, a)
            && !removeBundledArcFromList(grin->bundle, a)) {
        throw std::invalid_argument("Unknown outgoing arc.");
    }
}

void IncidenceListVertex::clearOutgoingArcs()
{
    grin->outgoingArcs.clear();
    grin->outgoingMultiArcs.clear();
    grin->outIndex.resetAll();
    grin->multiOutIndex.resetAll();
}

unsigned int IncidenceListVertex::getInDegree(bool multiArcsAsSimple) const
{
    unsigned int deg = grin->incomingArcs.size();
    if (multiArcsAsSimple) {
        return deg + grin->incomingMultiArcs.size();
    }
    for (MultiArc *ma : grin->incomingMultiArcs) {
        deg += ma->getSize();
    }
    return deg;
}

bool IncidenceListVertex::isSource() const
{
    return grin->incomingArcs.empty() && grin->incomingMultiArcs.empty();
}

bool IncidenceListVertex::isSink() const
{
    return grin->outgoingArcs.empty() && grin->outgoingMultiArcs.empty();
}

void IncidenceListVertex::addIncomingArc(Arc *a)
{
    if (grin->checkConsisteny && a->getHead() != this) {
        throw std::invalid_argument("Arc has other head.");
    }
    MultiArc *ma = dynamic_cast<MultiArc*>(a);
    if (ma) {
        grin->multiInIndex[ma] = grin->incomingMultiArcs.size();
        grin->incomingMultiArcs.push_back(ma);
        ParallelArcsBundle *pab = dynamic_cast<ParallelArcsBundle*>(ma);
        if (pab) {
            pab->mapArcs([&](Arc *a) {
                grin->bundle[a] = pab;
            });
        }
    } else {
        grin->inIndex[a] = grin->incomingArcs.size();
        grin->incomingArcs.push_back(a);
    }
}

void IncidenceListVertex::removeIncomingArc(const Arc *a)
{
    if (grin->checkConsisteny && a->getHead() != this) {
        throw std::invalid_argument("Arc has other head.");
    }
    if (!removeArcFromList(grin->incomingArcs, grin->inIndex, a)
            && !removeArcFromList(grin->incomingMultiArcs, grin->multiInIndex, a)
            && !removeBundledArcFromList(grin->bundle, a)) {
        throw std::invalid_argument("Unknown incoming arc.");
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

int IncidenceListVertex::getIndex() const
{
    return grin->index;
}

void IncidenceListVertex::setIndex(int i)
{
    grin->index = i;
}

bool IncidenceListVertex::hasOutgoingArc(const Arc *a) const
{
    return isArcInList(grin->outIndex, grin->outgoingArcs, a)
            || isArcInList(grin->multiOutIndex, grin->outgoingMultiArcs, a)
            || isBundledArc(grin->bundle, grin->outgoingMultiArcs, grin->multiOutIndex, a);
}

bool IncidenceListVertex::hasIncomingArc(const Arc *a) const
{
    return isArcInList(grin->inIndex, grin->incomingArcs, a)
            || isArcInList(grin->multiInIndex, grin->incomingMultiArcs, a)
            || isBundledArc(grin->bundle, grin->incomingMultiArcs, grin->multiInIndex, a);
}

Arc *IncidenceListVertex::outgoingArcAt(unsigned int i, bool multiArcsAsSimple) const
{
    if (i < grin->outgoingArcs.size()) {
        return grin->outgoingArcs.at(i);
    }
    i -= grin->outgoingArcs.size();
    if (multiArcsAsSimple) {
        if (i < grin->outgoingMultiArcs.size()) {
            return grin->outgoingMultiArcs.at(i);
        }
    } else {
        for(MultiArc *a : grin->outgoingMultiArcs) {
            if (i < a->getSize()) {
                return a;
            }
            i -= a->getSize();
        }
    }
    throw std::invalid_argument("Index must be less than outdegree.");
}

Arc *IncidenceListVertex::incomingArcAt(unsigned int i, bool multiArcsAsSimple) const
{
    if (i < grin->incomingArcs.size()) {
        return grin->incomingArcs.at(i);
    }
    i -= grin->incomingArcs.size();
    if (multiArcsAsSimple) {
        if (i < grin->incomingMultiArcs.size()) {
            return grin->incomingMultiArcs.at(i);
        }
    } else {
        for(MultiArc *a : grin->incomingMultiArcs) {
            if (i < a->getSize()) {
                return a;
            }
            i -= a->getSize();
        }
    }
    throw std::invalid_argument("Index must be less than indegree.");
}

int IncidenceListVertex::outIndexOf(const Arc *a) const
{
    auto i = grin->outIndex[a];
    if (i >= 0) {
        return i;
    }
    return grin->multiOutIndex[a];
}

int IncidenceListVertex::inIndexOf(const Arc *a) const
{
    auto i = grin->inIndex[a];
    if (i >= 0) {
        return i;
    }
    return grin->multiInIndex[a];
}

void IncidenceListVertex::acceptOutgoingArcVisitor(ArcVisitor *aVisitor) const
{
    mapOutgoingArcs(aVisitor->getVisitorFunction(), arcFalse);
}

void IncidenceListVertex::acceptIncomingArcVisitor(ArcVisitor *aVisitor) const
{
    mapIncomingArcs(aVisitor->getVisitorFunction(), arcFalse);
}

bool IncidenceListVertex::mapOutgoingArcs(const ArcMapping &avFun, const ArcPredicate &breakCondition, bool checkValidity) const
{
    for (Arc *a : grin->outgoingArcs) {
        if (breakCondition(a)) {
            return false;
        }
        if (!checkValidity || a->isValid()) {
            avFun(a);
        }
    }
    for (Arc *a : grin->outgoingMultiArcs) {
        if (breakCondition(a)) {
            return false;
        }
        if (!checkValidity || a->isValid()) {
            avFun(a);
        }
    }
    return true;
}

bool IncidenceListVertex::mapIncomingArcs(const ArcMapping &avFun, const ArcPredicate &breakCondition, bool checkValidity) const
{
    for (Arc *a : grin->incomingArcs) {
        if (breakCondition(a)) {
            return false;
        }
        if (!checkValidity || a->isValid()) {
            avFun(a);
        }
    }
    for (Arc *a : grin->incomingMultiArcs) {
        if (breakCondition(a)) {
            return false;
        }
        if (!checkValidity || a->isValid()) {
            avFun(a);
        }
    }
    return true;
}

template <typename AL>
bool removeArcFromList(AL &list, PropertyMap<int> &indexMap, const Arc *arc) {
    int i = indexMap(arc);
    if (i < 0) {
        return false;
    }
    assert(list[i] == arc);
    auto swap = list.back();
    list[i] = swap;
    indexMap[swap] = i;
    indexMap.resetToDefault(arc);
    list.pop_back();
    return true;
}

bool removeBundledArcFromList(PropertyMap<ParallelArcsBundle*> &bundleMap, const Arc *arc) {
    ParallelArcsBundle *pmb = bundleMap(arc);
    if (!pmb) {
        return false;
    }
    assert(pmb->containsArc(arc));
    pmb->removeArc(arc);
    bundleMap.resetToDefault(arc);
    return true;
}

template <typename AL>
bool isArcInList(const PropertyMap<int> &indexMap, AL &list, const Arc *arc) {
    bool found = indexMap(arc) >= 0;
    if (found) {
        assert(list[indexMap(arc)] == arc);
    }
    return found;
}

template <typename AL>
bool isBundledArc(const PropertyMap<ParallelArcsBundle*> bundleMap, AL &list, const PropertyMap<int> &indexMap, const Arc *arc) {
    ParallelArcsBundle *pmb = bundleMap(arc);
    if (!pmb) {
        return false;
    }
    assert(pmb->containsArc(arc));
    return isArcInList(indexMap, list, pmb);
}

}
