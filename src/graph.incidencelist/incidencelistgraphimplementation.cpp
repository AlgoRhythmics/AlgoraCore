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

#include "incidencelistgraphimplementation.h"

#include "graph/parallelarcsbundle.h"

#include "graph.visitor/vertexvisitor.h"
#include "graph.visitor/arcvisitor.h"
#include "graph.visitor/collectarcsvisitor.h"
#include "property/propertymap.h"

#include <vector>
#include <unordered_map>
#include <algorithm>


namespace Algora {

template<typename... Args>
bool any(Args... args) { return (... || args); }



IncidenceListGraphImplementation::IncidenceListGraphImplementation(DiGraph *handle)
    : graph(handle), numArcs(0U), nextVertexId(0U), nextArcId(0U)
{
    sharedOutIndexMap.setDefaultValue(NO_INDEX);
    sharedInIndexMap.setDefaultValue(NO_INDEX);
}

IncidenceListGraphImplementation::~IncidenceListGraphImplementation()
{
    clear(false);
    arcPool.clear();
    vertexPool.clear();
    // vertexStorage and arcStorage should be auto-destroyed.
}

IncidenceListGraphImplementation::IncidenceListGraphImplementation(const IncidenceListGraphImplementation &other, DiGraph *handle,
                                                                   ModifiableProperty<GraphArtifact *> *otherToThisVertices,
                                                                   ModifiableProperty<GraphArtifact *> *otherToThisArcs,
                                                                   ModifiableProperty<GraphArtifact *> *thisToOtherVertices,
                                                                   ModifiableProperty<GraphArtifact *> *thisToOtherArcs)
    : graph(nullptr), numArcs(0U), nextVertexId(0U), nextArcId(0U)
{
    if (handle != nullptr) {
        graph = handle;
    }
    if (any(otherToThisVertices == nullptr, otherToThisArcs == nullptr, thisToOtherVertices == nullptr, thisToOtherArcs == nullptr)) {
        PropertyMap<GraphArtifact*> pm;
        otherToThisVertices = otherToThisVertices == nullptr ? &pm : otherToThisVertices;
        thisToOtherVertices = thisToOtherVertices == nullptr ? &pm : thisToOtherVertices;
        otherToThisArcs = otherToThisArcs == nullptr ? &pm : otherToThisArcs;
        thisToOtherArcs = thisToOtherArcs == nullptr ? &pm : thisToOtherArcs;
        copyFrom(other, *otherToThisVertices, *otherToThisArcs, *thisToOtherVertices, *thisToOtherArcs);
    } else {
        copyFrom(other, *otherToThisVertices, *otherToThisArcs, *thisToOtherVertices, *thisToOtherArcs);
    }
}

IncidenceListGraphImplementation &IncidenceListGraphImplementation::assign(const IncidenceListGraphImplementation &other, DiGraph *handle, ModifiableProperty<GraphArtifact *> *otherToThisVertices, ModifiableProperty<GraphArtifact *> *otherToThisArcs, ModifiableProperty<GraphArtifact *> *thisToOtherVertices, ModifiableProperty<GraphArtifact *> *thisToOtherArcs)
{
    if (&other == this) {
        return *this;
    }

    if (handle != nullptr) {
        graph = handle;
    }
    if (any(otherToThisVertices == nullptr, otherToThisArcs == nullptr, thisToOtherVertices == nullptr, thisToOtherArcs == nullptr)) {
        PropertyMap<GraphArtifact*> pm;
        otherToThisVertices = otherToThisVertices == nullptr ? &pm : otherToThisVertices;
        thisToOtherVertices = thisToOtherVertices == nullptr ? &pm : thisToOtherVertices;
        otherToThisArcs = otherToThisArcs == nullptr ? &pm : otherToThisArcs;
        thisToOtherArcs = thisToOtherArcs == nullptr ? &pm : thisToOtherArcs;
    }
    copyFrom(other, *otherToThisVertices, *otherToThisArcs, *thisToOtherVertices, *thisToOtherArcs);

    return *this;
}

IncidenceListGraphImplementation::IncidenceListGraphImplementation(IncidenceListGraphImplementation &&other, DiGraph *handle)
    : IncidenceListGraphImplementation(std::move(other))
{
    if (handle != nullptr) {
        setOwner(handle);
    }
}

IncidenceListGraphImplementation &IncidenceListGraphImplementation::move(IncidenceListGraphImplementation &&other, DiGraph *handle)
{
    if (&other == this) {
        return *this;
    }

    operator=(std::move(other));
    if (handle != nullptr) {
        setOwner(handle);
    }

    return *this;
}

void IncidenceListGraphImplementation::clear(bool emptyReserves)
{
    for (IncidenceListVertex *v : vertices) {
        v->mapOutgoingArcs([this](Arc *a) {
            a->hibernate();
            arcPool.push_back(a);
        });
        v->clearOutgoingArcs();
        v->clearIncomingArcs();
        v->hibernate();
        vertexPool.push_back(v);
    }
    vertices.clear();
    numArcs = 0U;
    nextVertexId = 0U;
    nextArcId = 0U;
    recycledVertexIds.clear();
    recycledArcIds.clear();

    if (emptyReserves) {
        for (auto a : arcPool) {
            arcStorage.destroy(a);
        }
        arcPool.clear();
        for (auto v: vertexPool) {
            vertexStorage.destroy(v);
        }
        vertexPool.clear();
    }
}

void IncidenceListGraphImplementation::addVertex(IncidenceListVertex *vertex)
{
    vertex->setIndex(vertices.size());
    vertices.push_back(vertex);
}

void IncidenceListGraphImplementation::removeVertex(IncidenceListVertex *v)
{
    v->mapOutgoingArcs([this](Arc *a) {
        IncidenceListVertex *head = dynamic_cast<IncidenceListVertex*>(a->getHead());
        head->removeIncomingArc(a);
        //delete a;
        a->hibernate();
        arcPool.push_back(a);
    }, arcFalse, false);
    v->clearOutgoingArcs();
    v->mapIncomingArcs([this](Arc *a) {
        IncidenceListVertex *tail = dynamic_cast<IncidenceListVertex*>(a->getTail());
        tail->removeOutgoingArc(a);
        //delete a;
        a->hibernate();
        arcPool.push_back(a);
    }, arcFalse, false);
    v->clearIncomingArcs();
    IncidenceListVertex *o = vertices.back();
    auto index = v->getIndex();
    o->setIndex(index);
    vertices[index] = o;
    vertices.pop_back();
    v->hibernate();
    vertexPool.push_back(v);
}

bool IncidenceListGraphImplementation::containsVertex(const IncidenceListVertex *v) const
{
    return (v->getParent() == graph && vertices.at(v->getIndex()) == v);
}

IncidenceListVertex *IncidenceListGraphImplementation::getFirstVertex() const
{
    if (vertices.empty()) {
        return nullptr;
    }
    return vertices.at(0);
}

IncidenceListVertex *IncidenceListGraphImplementation::vertexAt(size_type i) const
{
    return vertices.at(i);
}

void IncidenceListGraphImplementation::addArc(Arc *a, IncidenceListVertex *tail, IncidenceListVertex *head)
{
    tail->addOutgoingArc(a);
    head->addIncomingArc(a);
    numArcs++;
}

void IncidenceListGraphImplementation::removeArc(Arc *a, IncidenceListVertex *tail, IncidenceListVertex *head)
{
    tail->removeOutgoingArc(a);
    head->removeIncomingArc(a);
    numArcs--;
    a->hibernate();
    arcPool.push_back(a);
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

IncidenceListGraphImplementation::size_type IncidenceListGraphImplementation::getNumArcs(bool multiArcsAsSimple) const
{
    if (multiArcsAsSimple) {
        return numArcs;
    }
    unsigned int arcWeights = 0U;
    for (auto v : vertices) {
        arcWeights += v->getOutDegree(multiArcsAsSimple);
    }
    return arcWeights;
}

IncidenceListGraphImplementation::size_type IncidenceListGraphImplementation::getOutDegree(
        const IncidenceListVertex *v, bool multiArcsAsSimple) const
{
    return v->getOutDegree(multiArcsAsSimple);
}

IncidenceListGraphImplementation::size_type IncidenceListGraphImplementation::getInDegree(
        const IncidenceListVertex *v, bool multiArcsAsSimple) const
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

void IncidenceListGraphImplementation::mapVertices(const VertexMapping &vvFun,
                                                   const VertexPredicate &breakCondition, bool checkValidity)
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

void IncidenceListGraphImplementation::mapArcs(const ArcMapping &avFun, const ArcPredicate &breakCondition)
{
    for (IncidenceListVertex *v : vertices) {
        if (!v->mapOutgoingArcs(avFun, breakCondition)) {
            break;
        }
    }
}

void IncidenceListGraphImplementation::mapOutgoingArcs(const IncidenceListVertex *v, const ArcMapping &avFun,
                                                         const ArcPredicate &breakCondition, bool checkValidity)
{
    v->mapOutgoingArcs(avFun, breakCondition, checkValidity);
}

void IncidenceListGraphImplementation::mapIncomingArcs(const IncidenceListVertex *v, const ArcMapping &avFun,
                                                         const ArcPredicate &breakCondition, bool checkValidity)
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

void IncidenceListGraphImplementation::reserveVertexCapacity(size_type n)
{
    if (n <= vertices.size() + vertexPool.size()) {
        return;
    }
    auto reserve = n - getSize();

    vertexPool.reserve(n);
    vertexStorage.set_next_size(reserve);

    std::vector<IncidenceListVertex*> tmp;
    tmp.reserve(reserve);
    for (auto i = 0ULL; i < reserve; i++) {
        auto v = createIncidenceListVertex();
        v->hibernate();
        tmp.push_back(v);
    }
    vertexPool.insert(vertexPool.end(), tmp.rbegin(), tmp.rend());
    vertices.reserve(getSize() + reserve);
}

void IncidenceListGraphImplementation::reserveArcCapacity(size_type n)
{
    if (n <= numArcs + arcPool.size()) {
        return;
    }
    auto reserve = n - numArcs;

    if (reserve == 0U) {
        return;
    }

    arcPool.reserve(n);
    arcStorage.set_next_size(reserve);

    std::vector<Arc*> tmp;
    tmp.reserve(reserve);
    for (auto i = 0ULL; i < reserve; i++) {
        auto a = createArc(nullptr, nullptr);
        a->hibernate();
        tmp.push_back(a);
    }
    arcPool.insert(arcPool.end(), tmp.rbegin(), tmp.rend());
}

IncidenceListVertex *IncidenceListGraphImplementation::recycleOrCreateIncidenceListVertex()
{
    if (!vertexPool.empty()) {
        auto v = vertexPool.back();
        vertexPool.pop_back();
        v->recycle();
        return v;
    }

    return createIncidenceListVertex();
}

IncidenceListVertex *IncidenceListGraphImplementation::createIncidenceListVertex()
{
    id_type id;
    if (recycledVertexIds.empty()) {
        id = nextVertexId++;
    } else  {
        id = recycledVertexIds.back();
        recycledVertexIds.pop_back();
    }
    // constructor takes only up to three parameters...?!
    auto v  = vertexStorage.construct(id, sharedOutIndexMap, sharedInIndexMap);
    v->setParent(graph);
    return v;
}

Arc *IncidenceListGraphImplementation::recycleOrCreateArc(IncidenceListVertex *tail, IncidenceListVertex *head)
{
    if (!arcPool.empty()) {
        auto a = arcPool.back();
        arcPool.pop_back();
        a->recycle(tail, head);
        return a;
    }

    return createArc(tail, head);
}

Arc *IncidenceListGraphImplementation::createArc(IncidenceListVertex *tail, IncidenceListVertex *head)
{
    id_type id;
    if (recycledArcIds.empty()) {
        id = nextArcId++;
    } else  {
        id = recycledArcIds.back();
        recycledArcIds.pop_back();
    }
    //return new Arc(tail, head, id, graph);
    Arc *arc = arcStorage.construct(id, graph);
    arc->recycle(tail, head);
    return arc;
}

IncidenceListGraphImplementation::id_type IncidenceListGraphImplementation::getNextArcId()
{
    if (recycledArcIds.empty()) {
        return nextArcId++;
    }
    auto id = recycledArcIds.back();
    recycledArcIds.pop_back();
    return id;
}

void IncidenceListGraphImplementation::setOwner(DiGraph *handle)
{
    if (handle == graph) {
        return;
    }

    graph = handle;

    for (auto *v : vertices) {
        v->setParent(graph);
        v->mapOutgoingArcs([this](Arc *a) { a->setParent(graph); });
    }
    for (auto *v : vertexPool) {
        v->setParent(graph);
    }
    for (auto *a : arcPool) {
        a->setParent(graph);
    }
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

void IncidenceListGraphImplementation::copyFrom(const IncidenceListGraphImplementation &other,
                                                ModifiableProperty<GraphArtifact *> &otherToThisVertices,
                                                ModifiableProperty<GraphArtifact *> &otherToThisArcs,
                                                ModifiableProperty<GraphArtifact *> &thisToOtherVertices,
                                                ModifiableProperty<GraphArtifact *> &thisToOtherArcs)
{
    clear(true);

    reserveVertexCapacity(other.vertices.size() + other.vertexPool.size());
    reserveArcCapacity(other.numArcs + other.arcPool.size());

    for (auto *v : other.vertices) {
        auto *cv = recycleOrCreateIncidenceListVertex();
        cv->setName(v->getName());
        otherToThisVertices[v] = cv;
        thisToOtherVertices[cv] = v;
        addVertex(cv);
    }

    for (auto *v : other.vertices) {
        auto *tail = dynamic_cast<IncidenceListVertex*>(otherToThisVertices(v));
        v->mapOutgoingArcs([&otherToThisVertices, &otherToThisArcs, &thisToOtherArcs, this, tail](Arc *a) {
            auto *head = dynamic_cast<IncidenceListVertex*>(otherToThisVertices(a->getHead()));
            auto *av = this->recycleOrCreateArc(tail, head);
            av->setName(a->getName());
            otherToThisArcs[a] = av;
            thisToOtherArcs[av] = a;
            addArc(av, tail, head);
        });
    }
}

}
