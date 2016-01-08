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

#include "incidencelistgraphimpl.h"

#include "incidencelistvertex.h"

#include "graph/arc.h"
#include "graph/parallelarcsbundle.h"

#include "graph.visitor/vertexvisitor.h"
#include "graph.visitor/arcvisitor.h"
#include "graph.visitor/collectarcsvisitor.h"

#include <vector>
#include <unordered_map>
#include <algorithm>

namespace Algora {

IncidenceListGraph::CheshireCat::CheshireCat()
{

}

IncidenceListGraph::CheshireCat::~CheshireCat()
{
    for (IncidenceListVertex *v : vertices) {
        v->visitOutgoingArcs([](Arc *a) { delete a; });
        v->clearOutgoingArcs();
        v->clearIncomingArcs();
        delete v;
    }
    vertices.clear();
}

void IncidenceListGraph::CheshireCat::addVertex(IncidenceListVertex *vertex)
{
    vertices.push_back(vertex);
}

void IncidenceListGraph::CheshireCat::removeVertex(IncidenceListVertex *v)
{
    v->visitOutgoingArcs([](Arc *a) {
        IncidenceListVertex *head = dynamic_cast<IncidenceListVertex*>(a->getHead());
        head->removeIncomingArc(a);
        delete a;
    });
    v->clearOutgoingArcs();
    v->visitIncomingArcs([](Arc *a) {
        IncidenceListVertex *tail = dynamic_cast<IncidenceListVertex*>(a->getTail());
        tail->removeOutgoingArc(a);
        delete a;
    });
    v->clearIncomingArcs();
    vertices.erase(std::find(vertices.cbegin(), vertices.cend(), v));
    delete v;
}

bool IncidenceListGraph::CheshireCat::containsVertex(IncidenceListVertex *v) const
{
    return std::find(vertices.cbegin(), vertices.cend(), v) != vertices.cend();
}

IncidenceListVertex *IncidenceListGraph::CheshireCat::getFirstVertex() const
{
    if (vertices.empty()) {
        return 0;
    }
    return vertices.at(0);
}

void IncidenceListGraph::CheshireCat::addArc(Arc *a, IncidenceListVertex *tail, IncidenceListVertex *head)
{
    tail->addOutgoingArc(a);
    head->addIncomingArc(a);
}

void IncidenceListGraph::CheshireCat::removeArc(Arc *a, IncidenceListVertex *tail, IncidenceListVertex *head)
{
    tail->removeOutgoingArc(a);
    head->removeIncomingArc(a);
    delete a;
}

bool IncidenceListGraph::CheshireCat::containsArc(Arc *a, IncidenceListVertex *tail) const
{
    return tail->hasOutgoingArc(a);
}

void IncidenceListGraph::CheshireCat::visitVertices(VertexVisitorFunc vvFun)
{
    for (Vertex *v : vertices) {
        vvFun(v);
    }
}

void IncidenceListGraph::CheshireCat::visitArcs(ArcVisitorFunc avFun)
{
    for (IncidenceListVertex *v : vertices) {
        v->visitOutgoingArcs(avFun);
    }
}

void IncidenceListGraph::CheshireCat::visitOutgoingArcs(const IncidenceListVertex *v, ArcVisitorFunc avFun)
{
    v->visitOutgoingArcs(avFun);
}

void IncidenceListGraph::CheshireCat::visitIncomingArcs(const IncidenceListVertex *v, ArcVisitorFunc avFun)
{
    v->visitIncomingArcs(avFun);
}

bool IncidenceListGraph::CheshireCat::isEmpty() const
{
    return vertices.empty();
}

int IncidenceListGraph::CheshireCat::getSize() const
{
    return vertices.size();
}

void IncidenceListGraph::CheshireCat::bundleParallelArcs()
{
    for (IncidenceListVertex *vertex : vertices) {
        vertex->clearIncomingArcs();
    }

    for (IncidenceListVertex *vertex : vertices) {
        bundleOutgoingArcs(vertex);
    }
}

void IncidenceListGraph::CheshireCat::bundleOutgoingArcs(IncidenceListVertex *vertex)
{
    std::vector<Arc*> outArcs;
    CollectArcsVisitor collector(&outArcs);
    vertex->acceptOutgoingArcVisitor(&collector);
    vertex->clearOutgoingArcs();

    std::unordered_map<IncidenceListVertex*,Arc*> map;
    for (Arc *outArc : outArcs) {
        IncidenceListVertex *head = dynamic_cast<IncidenceListVertex*>(outArc->getHead());
        if (map.count(head) == 0) {
            map[head] = outArc;
        } else {
            Arc *mappedArc = map[head];
            ParallelArcsBundle *bundle = dynamic_cast<ParallelArcsBundle*>(mappedArc);
            if (bundle) {
                bundle->addArc(outArc);
            } else {
                bundle = new ParallelArcsBundle(mappedArc);
                bundle->addArc(outArc);
                map[head] = bundle;
            }
        }
    }
    for (auto i : map) {
        Arc *arc = i.second;
        vertex->addOutgoingArc(arc);
        i.first->addIncomingArc(arc);
    }
}

}
