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

#ifndef INCIDENCELISTGRAPHIMPLEMENTATION_H
#define INCIDENCELISTGRAPHIMPLEMENTATION_H

#include "incidencelistgraph.h"
#include "incidencelistvertex.h"
#include "graph/arc.h"

#include "property/modifiableproperty.h"
#include "property/fastpropertymap.h"

#include <vector>
#include <boost/pool/object_pool.hpp>

namespace Algora {

class IncidenceListVertex;

typedef typename std::vector<IncidenceListVertex*> VertexList;

class IncidenceListGraphImplementation {

public:
    typedef GraphArtifact::size_type size_type;
    typedef GraphArtifact::id_type id_type;
    static constexpr size_type NO_INDEX = std::numeric_limits<size_type>::max();

    explicit IncidenceListGraphImplementation(DiGraph *handle);
    ~IncidenceListGraphImplementation();

    // copying
    IncidenceListGraphImplementation(const IncidenceListGraphImplementation &other,
                                     DiGraph *handle = nullptr,
                                     ModifiableProperty<GraphArtifact*> *otherToThisVertices = nullptr,
                                     ModifiableProperty<GraphArtifact*> *otherToThisArcs = nullptr,
                                     ModifiableProperty<GraphArtifact*> *thisToOtherVertices = nullptr,
                                     ModifiableProperty<GraphArtifact*> *thisToOtherArcs = nullptr
            );
    IncidenceListGraphImplementation& assign(const IncidenceListGraphImplementation &other,
                                             DiGraph *handle = nullptr,
                                             ModifiableProperty<GraphArtifact*> *otherToThisVertices = nullptr,
                                             ModifiableProperty<GraphArtifact*> *otherToThisArcs = nullptr,
                                             ModifiableProperty<GraphArtifact*> *thisToOtherVertices = nullptr,
                                             ModifiableProperty<GraphArtifact*> *thisToOtherArcs = nullptr
            );
    IncidenceListGraphImplementation& operator=(const IncidenceListGraphImplementation &other) { return assign(other); }

    // moving is costly, but better than copying
    IncidenceListGraphImplementation(IncidenceListGraphImplementation &&other) = default;
    IncidenceListGraphImplementation& operator=(IncidenceListGraphImplementation &&other) = default;
    IncidenceListGraphImplementation(IncidenceListGraphImplementation &&other, DiGraph *handle);
    IncidenceListGraphImplementation& move(IncidenceListGraphImplementation &&other, DiGraph *handle);

    void clear(bool emptyReserves = false);

    void addVertex(IncidenceListVertex *vertex);
    void removeVertex(IncidenceListVertex *v);
    bool containsVertex(const IncidenceListVertex *v) const;
    IncidenceListVertex *getFirstVertex() const;
    IncidenceListVertex *vertexAt(size_type i) const;

    void addArc(Arc *a, IncidenceListVertex *tail, IncidenceListVertex *head);
    void removeArc(Arc *a, IncidenceListVertex *tail, IncidenceListVertex *head);
    bool containsArc(const Arc *a, const IncidenceListVertex *tail) const;
    Arc *findArc(const IncidenceListVertex *tail, const IncidenceListVertex *head) const;
    size_type getNumArcs(bool multiArcsAsSimple = false) const;

    size_type getOutDegree(const IncidenceListVertex *v, bool multiArcsAsSimple = false) const;
    size_type getInDegree(const IncidenceListVertex *v, bool multiArcsAsSimple = false) const;
    bool isSource(const IncidenceListVertex *v) const;
    bool isSink(const IncidenceListVertex *v) const;

    void mapVertices(const VertexMapping &vvFun, const VertexPredicate &breakCondition, bool checkValidity = true);

    void mapArcs(const ArcMapping &avFun, const ArcPredicate &breakCondition);
    void mapOutgoingArcs(const IncidenceListVertex *v, const ArcMapping &avFun,
                         const ArcPredicate &breakCondition, bool checkValidity = true);
    void mapIncomingArcs(const IncidenceListVertex *v, const ArcMapping &avFun,
                         const ArcPredicate &breakCondition, bool checkValidity = true);

    bool isEmpty() const;
    Graph::size_type getSize() const;
    void bundleParallelArcs();
    void unbundleParallelArcs();

    void reserveVertexCapacity(size_type n);
    void reserveArcCapacity(size_type n);

    IncidenceListVertex *recycleOrCreateIncidenceListVertex();
    IncidenceListVertex *createIncidenceListVertex();
    Arc *recycleOrCreateArc(IncidenceListVertex *tail, IncidenceListVertex *head);
    Arc *createArc(IncidenceListVertex *tail, IncidenceListVertex *head);
    MultiArc *createMultiArc(IncidenceListVertex *tail, IncidenceListVertex *head, id_type);

    id_type getNextArcId();
    void setOwner(DiGraph *handle);

private:
    DiGraph *graph;
    VertexList vertices;
    size_type numArcs;
    id_type nextVertexId;
    id_type nextArcId;
    std::vector<id_type> recycledVertexIds;
    std::vector<id_type> recycledArcIds;

    boost::object_pool<IncidenceListVertex> vertexStorage;
    boost::object_pool<Arc> arcStorage;
    std::vector<IncidenceListVertex*> vertexPool;
    std::vector<Arc*> arcPool;
    std::vector<MultiArc*> multiArcs;

    FastPropertyMap<size_type> sharedOutIndexMap;
    FastPropertyMap<size_type> sharedInIndexMap;

    void bundleOutgoingArcs(IncidenceListVertex *vertex);
    void unbundleOutgoingArcs(IncidenceListVertex *vertex);

    void copyFrom(const IncidenceListGraphImplementation &other,
                  ModifiableProperty<GraphArtifact *> &otherToThisVertices,
                  ModifiableProperty<GraphArtifact *> &otherToThisArcs,
                  ModifiableProperty<GraphArtifact *> &thisToOtherVertices,
                  ModifiableProperty<GraphArtifact *> &thisToOtherArcs);
};

}

#endif // INCIDENCELISTGRAPHIMPLEMENTATION_H

