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


#ifndef GRAPH_H
#define GRAPH_H

#include "graphartifact.h"
#include "graph.visitor/visitorfunctions.h"

namespace Algora {

class Vertex;
class VertexVisitor;

class Graph : public GraphArtifact
{
public:
    explicit Graph(GraphArtifact *parent = 0);
    virtual ~Graph();

    // Vertices
    virtual Vertex *addVertex() = 0;
    virtual void removeVertex(Vertex *v) = 0;
    virtual bool containsVertex(Vertex *v) const = 0;
    virtual Vertex *getAnyVertex() const = 0;

    // Accomodate visitors
    virtual void acceptVertexVisitor(VertexVisitor *nVisitor);
    virtual void visitVertices(VertexVisitorFunc vvFun) = 0;

    // Misc
    virtual bool isEmpty() const = 0;
    virtual int getSize() const = 0;

protected:
    Vertex *createVertex();
};

}

#endif // GRAPH_H
