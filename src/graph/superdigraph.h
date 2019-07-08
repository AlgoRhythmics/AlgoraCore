/**
 * Copyright (C) 2013 - 2019 : Kathrin Hanauer
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
    explicit SuperDiGraph(DiGraph *graph);
    virtual ~SuperDiGraph() override;

    // Graph interface
public:
    virtual Vertex *addVertex() override;
    virtual void removeVertex(Vertex *v) override;
    virtual bool containsVertex(const Vertex *v) const override;
    virtual Vertex *getAnyVertex() const override;
    virtual void mapVerticesUntil(const VertexMapping &vvFun, const VertexPredicate &breakCondition) override;
    virtual bool isEmpty() const override;
    virtual size_type getSize() const override;

    virtual void clear() override;

    // DiGraph interface
public:
    virtual Arc *addArc(Vertex *tail, Vertex *head) override;
    virtual MultiArc *addMultiArc(Vertex *tail, Vertex *head, size_type size) override;
    virtual void removeArc(Arc *a) override;
    virtual bool containsArc(const Arc *a) const override;
    virtual Arc *findArc(const Vertex *from, const Vertex *to) const override;
    virtual size_type getOutDegree(const Vertex *v, bool multiArcsAsSimple = false) const override;
    virtual size_type getInDegree(const Vertex *v, bool multiArcsAsSimple = false) const override;
    virtual void mapArcsUntil(const ArcMapping &avFun, const ArcPredicate &breakCondition) override;
    virtual void mapOutgoingArcsUntil(const Vertex *v, const ArcMapping &avFun, const ArcPredicate &breakCondition) override;
    virtual void mapIncomingArcsUntil(const Vertex *v, const ArcMapping &avFun, const ArcPredicate &breakCondition) override;
    virtual size_type getNumArcs(bool multiArcsAsSimple = false) const override;

private:
    class CheshireCat;
    CheshireCat *grin;
};

}

#endif // SUPERDIGRAPH_H
