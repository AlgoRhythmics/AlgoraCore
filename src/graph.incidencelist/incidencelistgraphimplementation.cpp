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

IncidenceListGraphImplementation::IncidenceListGraphImplementation(DiGraph *handle)
    : graph(handle), nextVertexId(0U)
{

}

IncidenceListGraphImplementation::~IncidenceListGraphImplementation()
{
    for (IncidenceListVertex *v : vertices) {
        v->mapOutgoingArcs([](Arc *a) { delete a; });
        v->clearOutgoingArcs();
        v->clearIncomingArcs();
        delete v;
    }
    vertices.clear();
}

void IncidenceListGraphImplementation::addVertex(IncidenceListVertex *vertex)
{
    vertex->setIndex(vertices.size());
    vertices.push_back(vertex);
}

void IncidenceListGraphImplementation::removeVertex(IncidenceListVertex *v)
{
    v->mapOutgoingArcs([](Arc *a) {
        IncidenceListVertex *head = dynamic_cast<IncidenceListVertex*>(a->getHead());
        head->removeIncomingArc(a);
        delete a;
    }, arcFalse, false);
    v->clearOutgoingArcs();
    v->mapIncomingArcs([](Arc *a) {
        IncidenceListVertex *tail = dynamic_cast<IncidenceListVertex*>(a->getTail());
        tail->removeOutgoingArc(a);
        delete a;
    }, arcFalse, false);
    v->clearIncomingArcs();
    //vertices.erase(std::find(vertices.cbegin(), vertices.cend(), v));
    IncidenceListVertex *o = vertices.back();
    int index = v->getIndex();
    o->setIndex(index);
    vertices[index] = o;
    vertices.pop_back();
    delete v;
}

bool IncidenceListGraphImplementation::containsVertex(const IncidenceListVertex *v) const
{
    //return std::find(vertices.cbegin(), vertices.cend(), v) != vertices.cend();
    return (v->getParent() == graph && vertices.at(v->getIndex()) == v);
}

IncidenceListVertex *IncidenceListGraphImplementation::getFirstVertex() const
{
    if (vertices.empty()) {
        return 0;
    }
    return vertices.at(0);
}

IncidenceListVertex *IncidenceListGraphImplementation::vertexAt(unsigned int i) const
{
    return vertices.at(i);
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

bool IncidenceListGraphImplementation::containsArc(const Arc *a, const IncidenceListVertex *tail) const
{
    return tail->hasOutgoingArc(a);
}

Arc *IncidenceListGraphImplementation::findArc(const IncidenceListVertex *tail, const IncidenceListVertex *head) const
{
    Arc *arc = nullptr;
    tail->mapOutgoingArcs([&](Arc *a) {
        if (a->getHead() == head) {
            arc = a;
        }
    }, [&](const Arc*) { return arc != nullptr; });
    return arc;
}

int IncidenceListGraphImplementation::getOutDegree(const IncidenceListVertex *v, bool multiArcsAsSimple) const
{
    return v->getOutDegree(multiArcsAsSimple);
}

int IncidenceListGraphImplementation::getInDegree(const IncidenceListVertex *v, bool multiArcsAsSimple) const
{
    return v->getInDegree(multiArcsAsSimple);
}

bool IncidenceListGraphImplementation::isSource(const IncidenceListVertex *v) const
{
    return v->isSource();
}

bool IncidenceListGraphImplementation::isSink(const IncidenceListVertex *v) const
{
    return v->isSink();
}

void IncidenceListGraphImplementation::mapVertices(VertexMapping vvFun, VertexPredicate breakCondition, bool checkValidity)
{
    for (Vertex *v : vertices) {
        if (breakCondition(v)) {
            break;
        }
        if (!checkValidity || v->isValid()) {
            vvFun(v);
        }
    }
}

void IncidenceListGraphImplementation::mapArcs(ArcMapping avFun, ArcPredicate breakCondition)
{
    for (IncidenceListVertex *v : vertices) {
        if (!v->mapOutgoingArcs(avFun, breakCondition)) {
            break;
        }
    }
}

void IncidenceListGraphImplementation::mapOutgoingArcs(const IncidenceListVertex *v, ArcMapping avFun,
                                                         ArcPredicate breakCondition, bool checkValidity)
{
    v->mapOutgoingArcs(avFun, breakCondition, checkValidity);
}

void IncidenceListGraphImplementation::mapIncomingArcs(const IncidenceListVertex *v, ArcMapping avFun,
                                                         ArcPredicate breakCondition, bool checkValidity)
{
    v->mapIncomingArcs(avFun, breakCondition, checkValidity);
}

bool IncidenceListGraphImplementation::isEmpty() const
{
    return vertices.empty();
}

Graph::size_type IncidenceListGraphImplementation::getSize() const
{
    return vertices.size();
}

void IncidenceListGraphImplementation::bundleParallelArcs()
{
    unbundleParallelArcs();

    for (IncidenceListVertex *vertex : vertices) {
        vertex->clearIncomingArcs();
    }

    for (IncidenceListVertex *vertex : vertices) {
        bundleOutgoingArcs(vertex);
    }
}

void IncidenceListGraphImplementation::unbundleParallelArcs()
{
    for (IncidenceListVertex *vertex : vertices) {
        unbundleOutgoingArcs(vertex);
    }
}

IncidenceListVertex *IncidenceListGraphImplementation::createIncidenceListVertex()
{
    return new IncidenceListVertex(nextVertexId++, graph);
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

void IncidenceListGraphImplementation::unbundleOutgoingArcs(IncidenceListVertex *vertex)
{
    std::vector<Arc*> arcs;
    std::vector<ParallelArcsBundle*> arcBundles;
    vertex->mapOutgoingArcs([&](Arc *a) {
        ParallelArcsBundle *pab = dynamic_cast<ParallelArcsBundle*>(a);
        if (pab) {
            arcBundles.push_back(pab);
            pab->getArcs(&arcs);
            pab->clear();
        }
    });
    for (ParallelArcsBundle *pab : arcBundles) {
        removeArc(pab, vertex, dynamic_cast<IncidenceListVertex*>(pab->getHead()));
    }
    for (Arc *a : arcs) {
        addArc(a, vertex, dynamic_cast<IncidenceListVertex*>(a->getHead()));
    }
}

}
