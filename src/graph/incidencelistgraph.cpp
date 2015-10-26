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


#include "incidencelistgraph.h"
#include "vertex.h"
#include "arc.h"

#include "graph.visitor/vertexvisitor.h"
#include "graph.visitor/arcvisitor.h"

#include <list>
#include <unordered_map>

using namespace Algora;

struct IncidenceListVertex;
struct IncidenceListArc;

typedef std::list<IncidenceListVertex*> VertexList;
typedef std::list<IncidenceListArc*> ArcList;
typedef std::unordered_map<Vertex*,IncidenceListVertex*> VertexMap;
typedef std::unordered_map<Arc*,IncidenceListArc*> ArcMap;

struct IncidenceListVertex {
    Vertex *vertex;

    ArcList outgoingArcs;
    ArcList incomingArcs;

    IncidenceListVertex(Vertex *v)
      : vertex(v) { }
};

struct IncidenceListArc {
    Arc *arc;
    IncidenceListVertex *tail;
    IncidenceListVertex *head;

    IncidenceListArc(Arc *a, IncidenceListVertex *t, IncidenceListVertex *h)
        : arc(a), tail(t), head(h) { }
};

struct IncidenceListGraph::CheshireCat {
    VertexList vertices;
    VertexMap vertexMap;

    void addVertex(Vertex *v) {
        IncidenceListVertex *iv = new IncidenceListVertex(v);
        vertices.push_back(iv);
        vertexMap[iv->vertex] = iv;
    }

    void removeVertex(VertexMap::iterator &vMapIt) {
        IncidenceListVertex *iv = vMapIt->second;
        // remove incident arcs
        for (ArcList::const_iterator i = iv->outgoingArcs.cbegin();
             i != iv->outgoingArcs.cend(); i++) {
            IncidenceListArc *ila = *i;
            removeArcAtHead(ila);
            delete ila;
        }
        for (ArcList::const_iterator i = iv->incomingArcs.cbegin();
             i != iv->incomingArcs.cend(); i++) {
            IncidenceListArc *ila = *i;
            removeArcAtTail(ila);
            delete ila;
        }
        vertices.remove(iv);
        vertexMap.erase(vMapIt);
        delete iv->vertex;
        delete iv;
    }

    IncidenceListVertex *find(Vertex *v) {
        VertexMap::iterator i = vertexMap.find(v);
        if (i == vertexMap.end()) {
            return 0;
        }
        return i->second;
    }

    void addArc(Arc *a, IncidenceListVertex *t, IncidenceListVertex *h) {
        IncidenceListArc *ila = new IncidenceListArc(a, t, h);
        t->outgoingArcs.push_back(ila);
        h->incomingArcs.push_back(ila);
    }

    void removeArcAtTail(IncidenceListArc *ila) {
        ila->tail->outgoingArcs.remove(ila);
    }

    void removeArcAtHead(IncidenceListArc *ila) {
        ila->head->incomingArcs.remove(ila);
    }

    void removeArc(IncidenceListArc *ila) {
        removeArcAtTail(ila);
        removeArcAtHead(ila);
        delete ila->arc;
        delete ila;
    }
};

IncidenceListGraph::IncidenceListGraph(GraphArtifact *parent)
    : Graph(parent)
{
    cat = new CheshireCat;
}

IncidenceListGraph::~IncidenceListGraph()
{
    delete cat;
}

Vertex *IncidenceListGraph::addVertex()
{
    Vertex *v = new Vertex(this);
    cat->addVertex(v);
    return v;
}

bool IncidenceListGraph::removeVertex(Vertex *v)
{
    VertexMap::iterator i = cat->vertexMap.find(v);
    if (i == cat->vertexMap.end()) {
        return false;
    }
    cat->removeVertex(i);
    return true;
}

Arc *IncidenceListGraph::addArc(Vertex *tail, Vertex *head)
{
    IncidenceListVertex *t = cat->find(tail);
    IncidenceListVertex *h = cat->find(head);
    if (t == 0 || h == 0) {
        return 0;
    }
    Arc *a = new Arc(t->vertex, h->vertex, this);
    cat->addArc(a, t, h);
    return a;
}

bool IncidenceListGraph::removeArc(Arc *a)
{
    Vertex *tail = a->getTail();
    IncidenceListVertex *t = cat->find(tail);
    if (!t) {
        return false;
    }
    ArcList::iterator i = t->outgoingArcs.begin();
    for (;i != t->outgoingArcs.end(); i++) {
        if (a == (*i)->arc) {
            break;
        }
    }
    if (i == t->outgoingArcs.end()) {
        return false;
    }

    IncidenceListArc *ila = *i;
    cat->removeArcAtHead(ila);
    t->outgoingArcs.erase(i);
    delete ila->arc;
    delete ila;
    return true;
}

void IncidenceListGraph::acceptVertexVisitor(VertexVisitor *nVisitor)
{
    for (VertexList::const_iterator i = cat->vertices.cbegin();
         i != cat->vertices.cend(); i++) {
        IncidenceListVertex *ilv = *i;
        nVisitor->visitVertex(ilv->vertex);
    }
}

void IncidenceListGraph::acceptArcVisitor(ArcVisitor *aVisitor)
{
    for (VertexList::const_iterator i = cat->vertices.cbegin();
         i != cat->vertices.cend(); i++) {
        IncidenceListVertex *ilv = *i;
        for (ArcList::const_iterator j = ilv->outgoingArcs.cbegin();
             j != ilv->outgoingArcs.cend(); j++) {
            IncidenceListArc *ila = *j;
            aVisitor->visitArc(ila->arc);
        }
    }
}

void IncidenceListGraph::acceptOutgoingArcVisitor(Vertex *v, ArcVisitor *aVisitor)
{
    VertexMap::iterator i = cat->vertexMap.find(v);
    if (i == cat->vertexMap.end())
        return;

    IncidenceListVertex *ilv = i->second;
    for (ArcList::const_iterator j = ilv->outgoingArcs.cbegin();
         j != ilv->outgoingArcs.cend(); j++) {
        IncidenceListArc *ila = *j;
        aVisitor->visitArc(ila->arc);
    }
}

void IncidenceListGraph::acceptIncomingArcVisitor(Vertex *v, ArcVisitor *aVisitor)
{
    VertexMap::iterator i = cat->vertexMap.find(v);
    if (i == cat->vertexMap.end())
        return;

    IncidenceListVertex *ilv = i->second;
    for (ArcList::const_iterator j = ilv->incomingArcs.cbegin();
         j != ilv->incomingArcs.cend(); j++) {
        IncidenceListArc *ila = *j;
        aVisitor->visitArc(ila->arc);
    }
}

bool IncidenceListGraph::isEmpty() const
{
    return cat->vertices.empty();
}

int IncidenceListGraph::getSize() const
{
    return cat->vertices.size();
}