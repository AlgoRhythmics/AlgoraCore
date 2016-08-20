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


#ifndef SUBDIGRAPH_H
#define SUBDIGRAPH_H

#include "digraph.h"

namespace Algora {

template<typename T = bool>
class Property;

class SubDiGraph : public DiGraph
{
public:
    SubDiGraph(DiGraph *graph, Property<bool> &inherit);
    virtual ~SubDiGraph();

    // Graph interface
public:
    virtual Vertex *addVertex() override;
    virtual void removeVertex(Vertex *v) override;
    virtual bool containsVertex(Vertex *v) const override;
    virtual Vertex *getAnyVertex() const override;
    virtual void mapVerticesUntil(VertexMapping vvFun, VertexPredicate breakCondition) override;
    virtual bool isEmpty() const override;
    virtual size_type getSize() const override;

    // DiGraph interface
public:
    using DiGraph::mapArcs;
    using DiGraph::mapOutgoingArcsUntil;
    using DiGraph::mapIncomingArcsUntil;

    virtual DiGraph *createReversedGraph(PropertyMap<GraphArtifact*> &map) const override;
    virtual Arc *addArc(Vertex *tail, Vertex *head) override;
    virtual MultiArc *addMultiArc(Vertex *tail, Vertex *head, int size) override;
    virtual void removeArc(Arc *a) override;
    virtual bool containsArc(Arc *a) const override;
    virtual int getOutDegree(const Vertex *v, bool multiArcsAsSimple = false) const override;
    virtual int getInDegree(const Vertex *v, bool multiArcsAsSimple = false) const override;
    virtual void mapArcsUntil(ArcMapping avFun, ArcPredicate breakCondition) override;
    virtual void mapOutgoingArcsUntil(const Vertex *v, ArcMapping avFun, ArcPredicate breakCondition) override;
    virtual void mapIncomingArcsUntil(const Vertex *v, ArcMapping avFun, ArcPredicate breakCondition) override;

private:
    DiGraph *superGraph;
    Property<bool> &inSubGraph;

};

}

#endif // SUBDIGRAPH_H
