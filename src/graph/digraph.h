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


#ifndef DIGRAPH_H
#define DIGRAPH_H

#include "graph.h"
#include "arc.h"

#include "graph.visitor/arcvisitor.h"

namespace Algora {

class Vertex;

class DiGraph : public Graph
{
public:
    explicit DiGraph(GraphArtifact *parent = 0);
    virtual ~DiGraph() { }

    // Arcs
    virtual Arc *addArc(Vertex *tail, Vertex *head) = 0;
    virtual void removeArc(Arc *a) = 0;
    virtual bool containsArc(Arc *a) const = 0;

    virtual int getOutDegree(const Vertex *v) const = 0;
    virtual int getInDegree(const Vertex *v) const = 0;
    virtual int getNumArcs() const;

    virtual void onArcAdd(ArcVisitorFunc avFun) { arcGreetings.push_back(avFun); }
    virtual void onArcRemove(ArcVisitorFunc avFun) { arcFarewells.push_back(avFun); }

    // Accomodate visitors
    virtual void acceptArcVisitor(ArcVisitor *aVisitor) {
        visitArcs(aVisitor->getVisitorFunction());
    }
    virtual void acceptOutgoingArcVisitor(const Vertex *v, ArcVisitor *aVisitor) {
        visitOutgoingArcs(v, aVisitor->getVisitorFunction());
    }
    virtual void acceptIncomingArcVisitor(const Vertex *v, ArcVisitor *aVisitor) {
        visitIncomingArcs(v, aVisitor->getVisitorFunction());
    }
    virtual void visitArcs(ArcVisitorFunc avFun) {
        visitArcsUntil(avFun, arcFalse);
    }
    virtual void visitOutgoingArcs(const Vertex *v, ArcVisitorFunc avFun) { visitOutgoingArcsUntil(v, avFun, arcFalse); }
    virtual void visitIncomingArcs(const Vertex *v, ArcVisitorFunc avFun) { visitIncomingArcsUntil(v, avFun, arcFalse); }

    virtual void visitArcsUntil(ArcVisitorFunc avFun, ArcPredicate breakCondition) = 0;
    virtual void visitOutgoingArcsUntil(const Vertex *v, ArcVisitorFunc avFun, ArcPredicate breakCondition) = 0;
    virtual void visitIncomingArcsUntil(const Vertex *v, ArcVisitorFunc avFun, ArcPredicate breakCondition) = 0;

    // GraphArtifact interface
public:
    virtual std::string toString() const override;

protected:
   std::vector<ArcVisitorFunc> arcGreetings;
   std::vector<ArcVisitorFunc> arcFarewells;

   virtual void greetArc(Arc *a) {
       for (const ArcVisitorFunc &f : arcGreetings) { f(a); }
   }
   virtual void dismissArc(Arc *a) {
       for (const ArcVisitorFunc &f : arcFarewells) { f(a); }
   }

    Arc *createArc(Vertex *tail, Vertex *head) {
        return new Arc(tail, head, this);
    }
};

}

#endif // GRAPH_H
