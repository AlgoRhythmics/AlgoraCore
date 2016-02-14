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


#include "incidencelistgraphimplementation.h"

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

IncidenceListGraphImplementation::IncidenceListGraphImplementation(Graph *handle)
    : graph(handle)
{

}

IncidenceListGraphImplementation::~IncidenceListGraphImplementation()
{
    for (IncidenceListVertex *v : vertices) {
        v->visitOutgoingArcs([](Arc *a) { delete a; });
        v->clearOutgoingArcs();
        v->clearIncomingArcs();
        delete v;
    }
    vertices.clear();
}

void IncidenceListGraphImplementation::addVertex(IncidenceListVertex *vertex)
{
    vertices.push_back(vertex);
}

void IncidenceListGraphImplementation::removeVertex(IncidenceListVertex *v)
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

bool IncidenceListGraphImplementation::containsVertex(IncidenceListVertex *v) const
{
    return std::find(vertices.cbegin(), vertices.cend(), v) != vertices.cend();
}

IncidenceListVertex *IncidenceListGraphImplementation::getFirstVertex() const
{
    if (vertices.empty()) {
        return 0;
    }
    return vertices.at(0);
}

void IncidenceListGraphImplementation::addArc(Arc *a, IncidenceListVertex *tail, IncidenceListVertex *head)
{
    tail->addOutgoingArc(a);
    head->addIncomingArc(a);
}

void IncidenceListGraphImplementation::removeArc(Arc *a, IncidenceListVertex *tail, IncidenceListVertex *head)
{
    tail->removeOutgoingArc(a);
    head->removeIncomingArc(a);
    delete a;
}

bool IncidenceListGraphImplementation::containsArc(Arc *a, IncidenceListVertex *tail) const
{
    return tail->hasOutgoingArc(a);
}

int IncidenceListGraphImplementation::getOutDegree(const IncidenceListVertex *v) const
{
    return v->getOutDegree();
}

int IncidenceListGraphImplementation::getInDegree(const IncidenceListVertex *v) const
{
    return v->getInDegree();
}

void IncidenceListGraphImplementation::visitVertices(VertexVisitorFunc vvFun, VertexPredicate breakCondition)
{
    for (Vertex *v : vertices) {
        if (breakCondition(v)) {
            break;
        }
        vvFun(v);
    }
}

void IncidenceListGraphImplementation::visitArcs(ArcVisitorFunc avFun, ArcPredicate breakCondition)
{
    for (IncidenceListVertex *v : vertices) {
        if (!v->visitOutgoingArcs(avFun, breakCondition)) {
            break;
        }
    }
}

void IncidenceListGraphImplementation::visitOutgoingArcs(const IncidenceListVertex *v, ArcVisitorFunc avFun,
                                                         ArcPredicate breakCondition)
{
    v->visitOutgoingArcs(avFun, breakCondition);
}

void IncidenceListGraphImplementation::visitIncomingArcs(const IncidenceListVertex *v, ArcVisitorFunc avFun,
                                                         ArcPredicate breakCondition)
{
    v->visitIncomingArcs(avFun, breakCondition);
}

bool IncidenceListGraphImplementation::isEmpty() const
{
    return vertices.empty();
}

int IncidenceListGraphImplementation::getSize() const
{
    return vertices.size();
}

void IncidenceListGraphImplementation::bundleParallelArcs()
{
    for (IncidenceListVertex *vertex : vertices) {
        vertex->clearIncomingArcs();
    }

    for (IncidenceListVertex *vertex : vertices) {
        bundleOutgoingArcs(vertex);
    }
}

IncidenceListVertex *IncidenceListGraphImplementation::createIncidenceListVertex() const
{
    return new IncidenceListVertex(graph);
}

void IncidenceListGraphImplementation::bundleOutgoingArcs(IncidenceListVertex *vertex)
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
