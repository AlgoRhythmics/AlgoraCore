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

#ifndef INCIDENCELISTGRAPH_H
#define INCIDENCELISTGRAPH_H

#include "graph/digraph.h"

namespace Algora {

class IncidenceListVertex;
class IncidenceListGraphImplementation;

class IncidenceListGraph : public DiGraph
{
public:
    explicit IncidenceListGraph(GraphArtifact *parent = 0);
    virtual ~IncidenceListGraph();

    // Graph interface
public:
    virtual Vertex *addVertex() override;
    virtual void removeVertex(Vertex *v) override;
    virtual bool containsVertex(const Vertex *v) const override;
    virtual Vertex *getAnyVertex() const override;
    virtual IncidenceListVertex *vertexAt(unsigned int i) const;

    virtual void mapVerticesUntil(VertexMapping vvFun, VertexPredicate breakCondition) override;

    virtual bool isEmpty() const override;
    virtual size_type getSize() const override;

    virtual void clear() override;

    // DiGraph interface
public:
    DiGraph *createReversedGraph(PropertyMap<GraphArtifact *> &map) const;
    virtual Arc *addArc(Vertex *tail, Vertex *head) override;
    virtual MultiArc *addMultiArc(Vertex *tail, Vertex *head, int size) override;
    virtual void removeArc(Arc *a) override;
    virtual bool containsArc(const Arc *a) const override;
    virtual Arc *findArc(const Vertex *from, const Vertex *to) const override;

    virtual int getOutDegree(const Vertex *v, bool multiArcsAsSimple = false) const override;
    virtual int getInDegree(const Vertex *v, bool multiArcsAsSimple = false) const override;

    virtual void mapArcsUntil(ArcMapping avFun, ArcPredicate breakCondition) override;
    virtual void mapOutgoingArcsUntil(const Vertex *v, ArcMapping avFun, ArcPredicate breakCondition) override;
    virtual void mapIncomingArcsUntil(const Vertex *v, ArcMapping avFun, ArcPredicate breakCondition) override;

public:
    void bundleParallelArcs();
    void unbundleParallelArcs();

protected:
    IncidenceListVertex *createIncidenceListVertex();

private:
    IncidenceListGraphImplementation *impl;
};

}

#endif // INCIDENCELISTGRAPH_H
