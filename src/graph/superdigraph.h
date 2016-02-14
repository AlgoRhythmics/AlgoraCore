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


#ifndef SUPERDIGRAPH_H
#define SUPERDIGRAPH_H

#include "digraph.h"

namespace Algora {

class SuperDiGraph : public DiGraph
{
public:
    SuperDiGraph(DiGraph *graph);
    virtual ~SuperDiGraph();

    // Graph interface
public:
    virtual Vertex *addVertex() override;
    virtual void removeVertex(Vertex *v) override;
    virtual bool containsVertex(Vertex *v) const override;
    virtual Vertex *getAnyVertex() const override;
    virtual void visitVerticesUntil(VertexVisitorFunc vvFun, VertexPredicate breakCondition) override;
    virtual bool isEmpty() const override;
    virtual int getSize() const override;

    // DiGraph interface
public:
    virtual Arc *addArc(Vertex *tail, Vertex *head) override;
    virtual void removeArc(Arc *a) override;
    virtual bool containsArc(Arc *a) const override;
    virtual int getOutDegree(const Vertex *v) const override;
    virtual int getInDegree(const Vertex *v) const override;
    virtual void visitArcsUntil(ArcVisitorFunc avFun, ArcPredicate breakCondition) override;
    virtual void visitOutgoingArcsUntil(const Vertex *v, ArcVisitorFunc avFun, ArcPredicate breakCondition) override;
    virtual void visitIncomingArcsUntil(const Vertex *v, ArcVisitorFunc avFun, ArcPredicate breakCondition) override;

private:
    class CheshireCat;
    CheshireCat *grin;
};

}

#endif // SUPERDIGRAPH_H
