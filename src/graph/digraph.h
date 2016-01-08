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

namespace Algora {

class Vertex;
class Arc;

class VertexVisitor;
class ArcVisitor;

class DiGraph : public Graph
{
public:
    explicit DiGraph(GraphArtifact *parent = 0);
    virtual ~DiGraph();

    // Arcs
    virtual Arc *addArc(Vertex *tail, Vertex *head) = 0;
    virtual void removeArc(Arc *a) = 0;
    virtual bool containsArc(Arc *a) const = 0;

    // Accomodate visitors
    virtual void acceptArcVisitor(ArcVisitor *aVisitor);
    virtual void acceptOutgoingArcVisitor(const Vertex *v, ArcVisitor *aVisitor);
    virtual void acceptIncomingArcVisitor(const Vertex *v, ArcVisitor *aVisitor);
    virtual void visitArcs(ArcVisitorFunc avFun) = 0;
    virtual void visitOutgoingArcs(const Vertex *v, ArcVisitorFunc avFun) = 0;
    virtual void visitIncomingArcs(const Vertex *v, ArcVisitorFunc avFun) = 0;

    // GraphArtifact interface
public:
    virtual std::string toString() const override;

protected:
    Arc *createArc(Vertex *tail, Vertex *head);
};

}

#endif // GRAPH_H
