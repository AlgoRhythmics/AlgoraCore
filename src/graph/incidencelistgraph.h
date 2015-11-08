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


#ifndef INCIDENCELISTGRAPH_H
#define INCIDENCELISTGRAPH_H

#include "digraph.h"

namespace Algora {

class IncidenceListGraph : public DiGraph
{
public:
    explicit IncidenceListGraph(GraphArtifact *parent = 0);
    virtual ~IncidenceListGraph();

    // DiGraph interface
public:
    virtual Vertex *addVertex();
    virtual bool removeVertex(Vertex *v);
    virtual Arc *addArc(const Vertex *tail, const Vertex *head);
    virtual bool removeArc(Arc *a);
    virtual void acceptVertexVisitor(VertexVisitor *nVisitor);
    virtual void acceptArcVisitor(ArcVisitor *aVisitor);
    virtual void acceptOutgoingArcVisitor(const Vertex *v, ArcVisitor *aVisitor);
    virtual void acceptIncomingArcVisitor(const Vertex *v, ArcVisitor *aVisitor);
    virtual bool isEmpty() const;
    virtual int getSize() const;

public:
    void bundleParallelArcs();

private:
    struct CheshireCat;
    CheshireCat *cat;
};

}

#endif // INCIDENCELISTGRAPH_H
