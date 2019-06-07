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

#include "incidencelistgraph.h"
#include "incidencelistvertex.h"
#include "graph/arc.h"
#include "graph/parallelarcsbundle.h"
#include "property/propertymap.h"

#include "incidencelistgraphimplementation.h"

#include <stdexcept>

namespace Algora {

const IncidenceListVertex *castVertex(const Vertex *v, const IncidenceListGraph *graph);
IncidenceListVertex *castVertex(Vertex *v, const IncidenceListGraph *graph);
void checkVertex(const IncidenceListVertex *v, const IncidenceListGraph *graph);


IncidenceListGraph::IncidenceListGraph(GraphArtifact *parent)
    : DiGraph(parent), impl(new IncidenceListGraphImplementation(this))
{
}

IncidenceListGraph::~IncidenceListGraph()
{
    if (impl != nullptr) {
        delete impl;
    }
}

IncidenceListGraph::IncidenceListGraph(const IncidenceListGraph &other, ModifiableProperty<GraphArtifact *> *otherToThisVertices, ModifiableProperty<GraphArtifact *> *otherToThisArcs, ModifiableProperty<GraphArtifact *> *thisToOtherVertices, ModifiableProperty<GraphArtifact *> *thisToOtherArcs)
    : DiGraph(other), impl(new IncidenceListGraphImplementation(*other.impl, this, otherToThisVertices, otherToThisArcs, thisToOtherVertices, thisToOtherArcs))
{
}

IncidenceListGraph &IncidenceListGraph::operator=(const IncidenceListGraph &other)
{
    return assign(other);
}

IncidenceListGraph &IncidenceListGraph::assign(const IncidenceListGraph &other, ModifiableProperty<GraphArtifact *> *otherToThisVertices, ModifiableProperty<GraphArtifact *> *otherToThisArcs, ModifiableProperty<GraphArtifact *> *thisToOtherVertices, ModifiableProperty<GraphArtifact *> *thisToOtherArcs)
{
    if (&other == this) {
        return *this;
    }

    DiGraph::operator=(other);
    impl->assign(*other.impl, this, otherToThisVertices, otherToThisArcs, thisToOtherVertices, thisToOtherArcs);
    return *this;
}

IncidenceListGraph::IncidenceListGraph(IncidenceListGraph &&other)
    : DiGraph(std::move(other)), impl(other.impl)
{
    impl->setOwner(this);
    other.impl = nullptr;
}

IncidenceListGraph &IncidenceListGraph::operator=(IncidenceListGraph &&other)
{
    if (&other == this) {
        return *this;
    }
    DiGraph::operator=(std::move(other));
    impl->move(std::move(*other.impl), this);
    other.impl = nullptr;
    return *this;
}

Vertex *IncidenceListGraph::addVertex()
{
    auto v = recycleOrCreateIncidenceListVertex();
    impl->addVertex(v);
    greetVertex(v);
    return v;
}

void IncidenceListGraph::removeVertex(Vertex *v)
{
    auto vertex = castVertex(v, this);

    impl->mapOutgoingArcs(vertex, [&](Arc *a) {
        invalidateArc(a);
        dismissArc(a);
    }, arcFalse);
    impl->mapIncomingArcs(vertex, [&](Arc *a) {
        invalidateArc(a);
        dismissArc(a);
    }, arcFalse);

    invalidateVertex(vertex);
    dismissVertex(vertex);

    impl->removeVertex(vertex);
}

bool IncidenceListGraph::containsVertex(const Vertex *v) const
{
    if (v->getParent() != this) {
        return false;
    }

    auto vertex = dynamic_cast<const IncidenceListVertex*>(v);

    if (!vertex) {
        return false;
    }

    return impl->containsVertex(vertex);
}

Vertex *IncidenceListGraph::getAnyVertex() const
{
    return impl->getFirstVertex();
}

IncidenceListVertex *IncidenceListGraph::vertexAt(unsigned long long i) const
{
    if (i >= getSize()) {
        throw std::invalid_argument("Index must be less than graph size.");
    }
    return impl->vertexAt(i);
}

void IncidenceListGraph::mapVerticesUntil(const VertexMapping &vvFun, const VertexPredicate &breakCondition)
{
    impl->mapVertices(vvFun, breakCondition);
}

Arc *IncidenceListGraph::addArc(Vertex *tail, Vertex *head)
{
    auto t = castVertex(tail, this);
    auto h = castVertex(head, this);

    Arc *a = recycleOrCreateArc(t, h);

    impl->addArc(a, t, h);
    greetArc(a);
    return a;
}

MultiArc *IncidenceListGraph::addMultiArc(Vertex *tail, Vertex *head, unsigned long long size)
{
    if (size <= 0) {
        throw std::invalid_argument("Multiarcs must be of size at least 1.");
    }
    auto t = castVertex(tail, this);
    auto h = castVertex(head, this);

    MultiArc *a = createMultiArc(t, h, size, impl->getNextArcId());

    impl->addArc(a, t, h);
    greetArc(a);
    return a;
}

void IncidenceListGraph::removeArc(Arc *a)
{
    if (a->getParent() != this) {
        throw std::invalid_argument("Arc is not a part of this graph.");
    }

    auto tail = castVertex(a->getTail(), this);
    auto head = castVertex(a->getHead(), this);

    invalidateArc(a);
    dismissArc(a);
    impl->removeArc(a, tail, head);
}

bool IncidenceListGraph::containsArc(const Arc *a) const
{
    if (a->getParent() != this) {
        return false;
    }

    auto tail = dynamic_cast<IncidenceListVertex*>(a->getTail());
    if (!tail) {
        return false;
    }

    return impl->containsArc(a, tail);
}

Arc *IncidenceListGraph::findArc(const Vertex *from, const Vertex *to) const
{
    auto tail = castVertex(from, this);
    auto head = castVertex(to, this);
    return impl->findArc(tail, head);
}

unsigned long long IncidenceListGraph::getNumArcs(bool multiArcsAsSimple) const
{
    return impl->getNumArcs(multiArcsAsSimple);
}

unsigned long long IncidenceListGraph::getOutDegree(const Vertex *v, bool multiArcsAsSimple) const
{
    auto vertex = castVertex(v, this);
    return impl->getOutDegree(vertex, multiArcsAsSimple);
}

unsigned long long IncidenceListGraph::getInDegree(const Vertex *v, bool multiArcsAsSimple) const
{
    auto vertex = castVertex(v, this);
    return impl->getInDegree(vertex, multiArcsAsSimple);
}

bool IncidenceListGraph::isSource(const Vertex *v) const
{
    auto vertex = castVertex(v, this);
    return impl->isSource(vertex);
}

bool IncidenceListGraph::isSink(const Vertex *v) const
{
    auto vertex = castVertex(v, this);
    return impl->isSink(vertex);
}

void IncidenceListGraph::mapArcsUntil(const ArcMapping &avFun, const ArcPredicate &breakCondition)
{
    impl->mapArcs(avFun, breakCondition);
}

void IncidenceListGraph::mapOutgoingArcsUntil(const Vertex *v, const ArcMapping &avFun, const ArcPredicate &breakCondition)
{
    auto vertex = castVertex(v, this);
    impl->mapOutgoingArcs(vertex, avFun, breakCondition);
}

void IncidenceListGraph::mapIncomingArcsUntil(const Vertex *v, const ArcMapping &avFun, const ArcPredicate &breakCondition)
{
    auto vertex = castVertex(v, this);
    impl->mapIncomingArcs(vertex, avFun, breakCondition);
}

bool IncidenceListGraph::isEmpty() const
{
    return impl->isEmpty();
}

Graph::size_type IncidenceListGraph::getSize() const
{
    return impl->getSize();
}

void IncidenceListGraph::clear()
{
   impl->clear();
   DiGraph::clear();
}

void IncidenceListGraph::clearAndRelease()
{
   impl->clear(true);
   DiGraph::clear();
}

DiGraph *IncidenceListGraph::createReversedGraph(PropertyMap<GraphArtifact *> &map) const
{
    IncidenceListGraph *reversed = new IncidenceListGraph(this->getParent());
    impl->mapVertices([&](Vertex *v) {
        Vertex *vr = reversed->addVertex();
        map[v] = vr;
        map[vr] = v;
    } , vertexFalse);
    impl->mapArcs([&](Arc *a) {
        Arc *ar;
        Vertex *headr = dynamic_cast<Vertex*>(map[a->getTail()]);
        Vertex *tailr = dynamic_cast<Vertex*>(map[a->getHead()]);
        if (dynamic_cast<MultiArc*>(a)) {
            ar = reversed->addMultiArc(tailr, headr, a->getSize());
        } else {
            ar = reversed->addArc(tailr, headr);
        }
        map[a] = ar;
        map[ar] = a;
    }, arcFalse);
    return reversed;
}

void IncidenceListGraph::bundleParallelArcs()
{
    impl->bundleParallelArcs();
}

void IncidenceListGraph::unbundleParallelArcs()
{
    impl->unbundleParallelArcs();
}

void IncidenceListGraph::reserveVertexCapacity(unsigned long long n)
{
    impl->reserveVertexCapacity(n);
}

void IncidenceListGraph::reserveArcCapacity(unsigned long long n)
{
   impl->reserveArcCapacity(n);
}

IncidenceListVertex *IncidenceListGraph::recycleOrCreateIncidenceListVertex()
{
    return impl->recycleOrCreateIncidenceListVertex();
}

IncidenceListVertex *IncidenceListGraph::createIncidenceListVertex()
{
    return impl->createIncidenceListVertex();
}

Arc *IncidenceListGraph::recycleOrCreateArc(IncidenceListVertex *tail, IncidenceListVertex *head)
{
    return impl->recycleOrCreateArc(tail, head);
}

Arc *IncidenceListGraph::createArc(IncidenceListVertex *tail, IncidenceListVertex *head)
{
    return impl->createArc(tail, head);
}

const IncidenceListVertex *castVertex(const Vertex *v, const IncidenceListGraph *graph) {
    auto vertex = dynamic_cast<const IncidenceListVertex*>(v);
    checkVertex(vertex, graph);
    return vertex;
}

IncidenceListVertex *castVertex(Vertex *v, const IncidenceListGraph *graph) {
    auto vertex = dynamic_cast<IncidenceListVertex*>(v);
    checkVertex(vertex, graph);
    return vertex;
}

void checkVertex(const IncidenceListVertex *v, const IncidenceListGraph *graph) {
    if (!v || v->getParent() != graph || graph->vertexAt(v->getIndex()) != v) {
        throw std::invalid_argument("Vertex is not a part of this graph.");
    }
}

}
