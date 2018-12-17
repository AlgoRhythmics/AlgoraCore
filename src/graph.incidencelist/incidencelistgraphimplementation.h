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

#ifndef INCIDENCELISTGRAPHIMPLEMENTATION_H
#define INCIDENCELISTGRAPHIMPLEMENTATION_H

#include "incidencelistgraph.h"
#include <vector>

namespace Algora {

class IncidenceListVertex;

typedef typename std::vector<IncidenceListVertex*> VertexList;

class IncidenceListGraphImplementation {

public:
    explicit IncidenceListGraphImplementation(DiGraph *handle);
    ~IncidenceListGraphImplementation();
    void clear();

    void addVertex(IncidenceListVertex *vertex);
    void removeVertex(IncidenceListVertex *v);
    bool containsVertex(const IncidenceListVertex *v) const;
    IncidenceListVertex *getFirstVertex() const;
    IncidenceListVertex *vertexAt(unsigned int i) const;

    void addArc(Arc *a, IncidenceListVertex *tail, IncidenceListVertex *head);
    void removeArc(Arc *a, IncidenceListVertex *tail, IncidenceListVertex *head);
    bool containsArc(const Arc *a, const IncidenceListVertex *tail) const;
    Arc *findArc(const IncidenceListVertex *tail, const IncidenceListVertex *head) const;
    unsigned int getNumArcs(bool multiArcsAsSimple = false) const;

    unsigned long long getOutDegree(const IncidenceListVertex *v, bool multiArcsAsSimple = false) const;
    unsigned long long getInDegree(const IncidenceListVertex *v, bool multiArcsAsSimple = false) const;
    bool isSource(const IncidenceListVertex *v) const;
    bool isSink(const IncidenceListVertex *v) const;

    void mapVertices(const VertexMapping &vvFun, const VertexPredicate &breakCondition, bool checkValidity = true);

    void mapArcs(const ArcMapping &avFun, const ArcPredicate &breakCondition);
    void mapOutgoingArcs(const IncidenceListVertex *v, const ArcMapping &avFun, const ArcPredicate &breakCondition, bool checkValidity = true);
    void mapIncomingArcs(const IncidenceListVertex *v, const ArcMapping &avFun, const ArcPredicate &breakCondition, bool checkValidity = true);

    bool isEmpty() const;
    Graph::size_type getSize() const;
    void bundleParallelArcs();
    void unbundleParallelArcs();

    IncidenceListVertex *createIncidenceListVertex();
    Arc *createArc(IncidenceListVertex *tail, IncidenceListVertex *head);

    unsigned int getNextArcId();

private:
    DiGraph *graph;
    VertexList vertices;
    unsigned long long numArcs;
    unsigned long long nextVertexId;
    unsigned long long nextArcId;
    std::vector<unsigned long long int> recycledVertexIds;
    std::vector<unsigned long long int> recycledArcIds;

    std::vector<IncidenceListVertex*> vertexPool;
    std::vector<Arc*> arcPool;

    void bundleOutgoingArcs(IncidenceListVertex *vertex);
    void unbundleOutgoingArcs(IncidenceListVertex *vertex);
};

}

#endif // INCIDENCELISTGRAPHIMPLEMENTATION_H

