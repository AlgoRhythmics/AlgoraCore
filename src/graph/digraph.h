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

#ifndef DIGRAPH_H
#define DIGRAPH_H

#include "graph.h"
#include "arc.h"
#include "weightedarc.h"
#include "parallelarcsbundle.h"

#include "graph.visitor/arcvisitor.h"

namespace Algora {

class Vertex;
template <typename T>
class PropertyMap;

class DiGraph : public Graph
{
public:
    explicit DiGraph(GraphArtifact *parent = 0);
    virtual ~DiGraph() { }

    virtual DiGraph *createReversedGraph(PropertyMap<GraphArtifact*> &) const { return 0; }

    // Arcs
    virtual Arc *addArc(Vertex *tail, Vertex *head) = 0;
    virtual MultiArc *addMultiArc(Vertex *tail, Vertex *head, unsigned long long size) = 0;
    virtual void removeArc(Arc *a) = 0;
    virtual bool containsArc(const Arc *a) const = 0;
    virtual Arc *findArc(const Vertex *from, const Vertex *to) const = 0;

    virtual unsigned long long getOutDegree(const Vertex *v, bool multiArcsAsSimple = false) const = 0;
    virtual unsigned long long getInDegree(const Vertex *v, bool multiArcsAsSimple = false) const = 0;
    virtual unsigned long long getDegree(const Vertex *v, bool multiArcsAsSimple = false) const {
        return getOutDegree(v, multiArcsAsSimple) + getInDegree(v, multiArcsAsSimple);
    }
    virtual bool isSource(const Vertex *v) const {
        return getInDegree(v) == 0;
    }
    virtual bool isSink(const Vertex *v) const {
        return getOutDegree(v) == 0;
    }
    virtual bool isIsolated(const Vertex *v) const {
        return isSource(v) && isSink(v);
    }
    virtual unsigned long long getNumArcs(bool multiArcsAsSimple = false) const;

    virtual void onArcAdd(void *id, const ArcMapping &avFun) { arcGreetings.push_back(std::make_pair(id, avFun)); }
    virtual void onArcRemove(void *id, const ArcMapping &avFun) { arcFarewells.push_back(std::make_pair(id, avFun)); }
    virtual void removeOnArcAdd(void *id) {
        auto i = arcGreetings.begin();
        while (i != arcGreetings.end()) {
            if (id == i->first) {
                i = arcGreetings.erase(i);
            } else {
                i++;
            }
        }
    }
    virtual void removeOnArcRemove(void *id) {
        auto i = arcFarewells.begin();
        while (i != arcFarewells.end()) {
            if (id == i->first) {
                i = arcFarewells.erase(i);
            } else {
                i++;
            }
        }
    }

    // Accomodate visitors
    virtual void acceptArcVisitor(ArcVisitor *aVisitor) {
        mapArcs(aVisitor->getVisitorFunction());
    }
    virtual void acceptOutgoingArcVisitor(const Vertex *v, ArcVisitor *aVisitor) {
        mapOutgoingArcs(v, aVisitor->getVisitorFunction());
    }
    virtual void acceptIncomingArcVisitor(const Vertex *v, ArcVisitor *aVisitor) {
        mapIncomingArcs(v, aVisitor->getVisitorFunction());
    }
    virtual void mapArcs(const ArcMapping &avFun) {
        mapArcsUntil(avFun, arcFalse);
    }
    virtual void mapOutgoingArcs(const Vertex *v, const ArcMapping &avFun) { mapOutgoingArcsUntil(v, avFun, arcFalse); }
    virtual void mapIncomingArcs(const Vertex *v, const ArcMapping &avFun) { mapIncomingArcsUntil(v, avFun, arcFalse); }

    virtual void mapArcsUntil(const ArcMapping &avFun, const ArcPredicate &breakCondition) = 0;
    virtual void mapOutgoingArcsUntil(const Vertex *v, const ArcMapping &avFun, const ArcPredicate &breakCondition) = 0;
    virtual void mapIncomingArcsUntil(const Vertex *v, const ArcMapping &avFun, const ArcPredicate &breakCondition) = 0;

    virtual void clear() override { arcGreetings.clear(); arcFarewells.clear(); Graph::clear(); }

    // GraphArtifact interface
public:
    virtual std::string toString() const override;

protected:
   std::vector<std::pair<void*,ArcMapping>> arcGreetings;
   std::vector<std::pair<void*,ArcMapping>> arcFarewells;

   virtual void greetArc(Arc *a) {
       for (const auto &p : arcGreetings) { p.second(a); }
   }
   virtual void dismissArc(Arc *a) {
       for (const auto &p : arcFarewells) { p.second(a); }
   }

    virtual Arc *createArc(Vertex *tail, Vertex *head) {
        return new Arc(tail, head, this);
    }

    virtual Arc *createArc(Vertex *tail, Vertex *head, unsigned long long id) {
        return new Arc(tail, head, id, this);
    }

    virtual MultiArc *createMultiArc(Vertex *tail, Vertex *head, unsigned long long size) {
        return new WeightedArc(tail, head, size, this);
    }

    virtual MultiArc *createMultiArc(Vertex *tail, Vertex *head, unsigned long long size, unsigned long long id) {
        return new WeightedArc(tail, head, size, id, this);
    }

    void invalidateArc(Arc *a) {
        a->invalidate();
    }
};

}

#endif // DIGRAPH_H
