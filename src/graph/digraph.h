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


#ifndef DIGRAPH_H
#define DIGRAPH_H

#include "graphartifact.h"

namespace Algora {

class Vertex;
class Arc;

class VertexVisitor;
class ArcVisitor;

class DiGraph : public GraphArtifact
{
public:
    explicit DiGraph(GraphArtifact *parent = 0);
    virtual ~DiGraph();

    // Vertices
    virtual Vertex *addVertex() = 0;
    virtual bool removeVertex(Vertex *v) = 0;

    // Arcs
    virtual Arc *addArc(const Vertex *tail, const Vertex *head) = 0;
    virtual bool removeArc(Arc *a) = 0;

    // Accomodate visitors
    virtual void acceptVertexVisitor(VertexVisitor *nVisitor) = 0;
    virtual void acceptArcVisitor(ArcVisitor *aVisitor) = 0;
    virtual void acceptOutgoingArcVisitor(const Vertex *v, ArcVisitor *aVisitor) = 0;
    virtual void acceptIncomingArcVisitor(const Vertex *v, ArcVisitor *aVisitor) = 0;

    // Misc
    virtual bool isEmpty() const = 0;
    virtual int getSize() const = 0;

    // GraphArtifact interface
public:
    virtual std::string toString() const override;
};

}

#endif // GRAPH_H
