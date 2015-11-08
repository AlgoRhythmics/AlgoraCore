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
#include "parallelarcsbundle.h"

#include "graph.visitor/vertexvisitor.h"
#include "graph.visitor/arcvisitor.h"

#include <vector>
#include <unordered_map>
#include <algorithm>

using namespace Algora;

struct IncidenceListVertex;
struct IncidenceListArc;

typedef std::vector<IncidenceListVertex*> VertexList;
typedef std::vector<IncidenceListArc*> ArcList;
typedef std::unordered_map<const Vertex*,IncidenceListVertex*> VertexMap;
typedef std::unordered_map<const Arc*,IncidenceListArc*> ArcMap;

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

    ~CheshireCat() {
        for (auto vIter = vertices.cbegin(); vIter != vertices.cend(); vIter++) {
            IncidenceListVertex *iv = *vIter;
            for (auto aIter = iv->outgoingArcs.cbegin(); aIter != iv->outgoingArcs.cend(); aIter++) {
                delete *aIter;
            }
            iv->outgoingArcs.clear();
            iv->incomingArcs.clear();
            delete iv->vertex;
            delete iv;
        }
        vertices.clear();
        vertexMap.clear();
    }

    void addVertex(Vertex *v) {
        IncidenceListVertex *iv = new IncidenceListVertex(v);
        vertices.push_back(iv);
        vertexMap[iv->vertex] = iv;
    }

    void removeVertex(VertexMap::iterator &vMapIt) {
        IncidenceListVertex *iv = vMapIt->second;
        // remove incident arcs
        for (auto i = iv->outgoingArcs.cbegin();
             i != iv->outgoingArcs.cend(); i++) {
            IncidenceListArc *ila = *i;
            removeArcAtHead(ila);
            delete ila;
        }
        for (auto i = iv->incomingArcs.cbegin();
             i != iv->incomingArcs.cend(); i++) {
            IncidenceListArc *ila = *i;
            removeArcAtTail(ila);
            delete ila;
        }
        vertices.erase(std::remove(vertices.begin(), vertices.end(), iv), vertices.end());
        vertexMap.erase(vMapIt);
        delete iv->vertex;
        delete iv;
    }

    IncidenceListVertex *find(const Vertex *v) {
        try {
          return vertexMap.at(v);
        } catch (const std::out_of_range& ) {
            return 0;
        }
    }

    void addArc(Arc *a, IncidenceListVertex *t, IncidenceListVertex *h) {
        IncidenceListArc *ila = new IncidenceListArc(a, t, h);
        t->outgoingArcs.push_back(ila);
        h->incomingArcs.push_back(ila);
    }

    void removeArcAtTail(IncidenceListArc *ila) {
        ila->tail->outgoingArcs.erase(std::remove(
                                          ila->tail->outgoingArcs.begin(),
                                          ila->tail->outgoingArcs.end(), ila),
                                      ila->tail->outgoingArcs.end());
    }

    void removeArcAtHead(IncidenceListArc *ila) {
        ila->tail->incomingArcs.erase(std::remove(
                                          ila->tail->incomingArcs.begin(),
                                          ila->tail->incomingArcs.end(), ila),
                                      ila->tail->incomingArcs.end());
    }

    void removeArc(IncidenceListArc *ila) {
        removeArcAtTail(ila);
        removeArcAtHead(ila);
        delete ila->arc;
        delete ila;
    }
};

IncidenceListGraph::IncidenceListGraph(GraphArtifact *parent)
    : DiGraph(parent)
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

Arc *IncidenceListGraph::addArc(const Vertex *tail, const Vertex *head)
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
    auto i = t->outgoingArcs.begin();
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
    for (auto i = cat->vertices.cbegin();
         i != cat->vertices.cend(); i++) {
        IncidenceListVertex *ilv = *i;
        for (auto j = ilv->outgoingArcs.cbegin();
             j != ilv->outgoingArcs.cend(); j++) {
            IncidenceListArc *ila = *j;
            aVisitor->visitArc(ila->arc);
        }
    }
}

void IncidenceListGraph::acceptOutgoingArcVisitor(const Vertex *v, ArcVisitor *aVisitor)
{
    auto i = cat->vertexMap.find(v);
    if (i == cat->vertexMap.end())
        return;

    IncidenceListVertex *ilv = i->second;
    for (auto j = ilv->outgoingArcs.cbegin();
         j != ilv->outgoingArcs.cend(); j++) {
        IncidenceListArc *ila = *j;
        aVisitor->visitArc(ila->arc);
    }
}

void IncidenceListGraph::acceptIncomingArcVisitor(const Vertex *v, ArcVisitor *aVisitor)
{
    auto i = cat->vertexMap.find(v);
    if (i == cat->vertexMap.end())
        return;

    IncidenceListVertex *ilv = i->second;
    for (auto j = ilv->incomingArcs.cbegin();
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

void IncidenceListGraph::bundleParallelArcs()
{
    for (auto vIter = cat->vertices.cbegin(); vIter != cat->vertices.cend(); vIter++) {
        IncidenceListVertex *iv = *vIter;
        iv->incomingArcs.clear();
    }
    for (auto vIter = cat->vertices.cbegin(); vIter != cat->vertices.cend(); vIter++) {
        IncidenceListVertex *iv = *vIter;
        std::unordered_map<IncidenceListVertex*,IncidenceListArc*> map;
        for (auto aIter = iv->outgoingArcs.cbegin(); aIter != iv->outgoingArcs.cend(); aIter++) {
            IncidenceListArc *ia = *aIter;
            auto f = map.find(ia->head);
            if (f == map.end()) {
                map[ia->head] = ia;
            } else {
                Arc *a = f->second->arc;
                ParallelArcsBundle *pa = dynamic_cast<ParallelArcsBundle*>(a);
                if (pa == 0) {
                    pa = new ParallelArcsBundle(a->getTail(), a->getHead(), this);
                    pa->addArc(a);
                    f->second->arc = pa;
                }
                pa->addArc(ia->arc);
            }
        }
        iv->outgoingArcs.clear();

        for (auto mIter = map.cbegin(); mIter != map.cend(); mIter++) {
            IncidenceListArc *ia = mIter->second;
            iv->outgoingArcs.push_back(ia);
            ia->head->incomingArcs.push_back(ia);
        }
    }
}
