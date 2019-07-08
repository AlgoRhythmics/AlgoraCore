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

#ifndef GRAPH_H
#define GRAPH_H

#include "graphartifact.h"
#include "vertex.h"

#include "graph.visitor/vertexvisitor.h"
#include "graph_functional.h"
#include "observable.h"

#include <vector>

namespace Algora {

class Vertex;

class Graph : public GraphArtifact
{
public:
    //typedef GraphArtifact::size_type size_type;

    explicit Graph(GraphArtifact *parent = nullptr);
    virtual ~Graph() { }

    // copying
    Graph(const Graph &other);
    Graph &operator=(const Graph &other);

    // moving
    Graph(Graph &&other) = default;
    Graph &operator=(Graph &&other) = default;

    // Vertices
    virtual Vertex *addVertex() = 0;
    virtual void removeVertex(Vertex *v) = 0;
    virtual bool containsVertex(const Vertex *v) const = 0;
    virtual Vertex *getAnyVertex() const = 0;

    virtual void onVertexAdd(void *id, const VertexMapping &vvFun);
    virtual void onVertexRemove(void *id, const VertexMapping &vvFun);
    virtual void removeOnVertexAdd(void *id);
    virtual void removeOnVertexRemove(void *id);

    // Accomodate visitors
    virtual void acceptVertexVisitor(VertexVisitor *nVisitor) {
        mapVertices(nVisitor->getVisitorFunction());
    }
    virtual void mapVertices(const VertexMapping &vvFun) {
        mapVerticesUntil(vvFun, vertexFalse);
    }
    virtual void mapVerticesUntil(const VertexMapping &vvFun, const VertexPredicate &breakCondition) = 0;

    // Misc
    virtual bool isEmpty() const = 0;
    virtual size_type getSize() const = 0;

    virtual void clear();

protected:
   Observable<Vertex*> observableVertexGreetings;
   Observable<Vertex*> observableVertexFarewells;

   void greetVertex(Vertex *v) { observableVertexGreetings.notifyObservers(v); }
   void dismissVertex(Vertex *v) { observableVertexFarewells.notifyObservers(v); }

    Vertex *createVertex() {
        return new Vertex(this);
    }

    Vertex *createVertex(id_type id) {
        return new Vertex(id, this);
    }

    void invalidateVertex(Vertex *v) {
        v->invalidate();
    }

};

}

#endif // GRAPH_H
