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

#ifndef FINDDIPATHALGORITHM_H
#define FINDDIPATHALGORITHM_H

#include "algorithm/valuecomputingalgorithm.h"
#include "property/propertymap.h"
#include "graph/digraph.h"

#include <vector>

namespace Algora {

class Vertex;
class Arc;

template<template <typename T> typename property_map_type = PropertyMap>
class FindDiPathAlgorithm : public ValueComputingAlgorithm<bool>
{
public:
    typedef std::vector<Vertex*>::const_iterator VertexIterator;
    typedef std::vector<Arc*>::const_iterator ArcIterator;

    explicit FindDiPathAlgorithm(bool constructVertexPath = true,
                                 bool constructArcPath = true,
                                 bool twoWaySearch = true);
    virtual ~FindDiPathAlgorithm() override = default;

    void setConstructPaths(bool vertexPath, bool arcPath) {
        constructVertexPath = vertexPath;
        constructArcPath = arcPath;
    }

    void useTwoWaySearch(bool twoWay) {
        twoWaySearch = twoWay;
    }

    void setTwoWayStepSize(unsigned long stepSize) {
        twoWayStepSize = stepSize;
    }

    void setSourceVertex(Vertex *s) {
        from = s;
    }

    void setTargetVertex(Vertex *t) {
        to = t;
    }

    void setSourceAndTarget(Vertex *source, Vertex *target) {
        from = source;
        to = target;
    }

    ArcIterator begin() const {
        return arcsOnPathBegin();
    }

    ArcIterator end() const {
        return arcsOnPathEnd();
    }

    VertexIterator verticesOnPathBegin() const {
        return vertexPath.cbegin();
    }

    VertexIterator verticesOnPathEnd() const {
        return vertexPath.cend();
    }

    ArcIterator arcsOnPathBegin() const {
        return arcPath.cbegin();
    }

    ArcIterator arcsOnPathEnd() const {
        return arcPath.cend();
    }

    std::vector<Vertex*> deliverVerticesOnPath() {
        std::vector<Vertex*> empty;
        std::swap(vertexPath, empty);
        return empty;
    }

    std::vector<Arc*> deliverArcsOnPath() {
        std::vector<Arc*> empty;
        std::swap(arcPath, empty);
        return empty;
    }

    DiGraph::size_type getNumVerticesSeen() const {
        return pr_num_vertices_seen;
    }

    // DiGraphAlgorithm interface
public:
    virtual bool prepare() override;
    virtual void run() override;
    virtual std::string getName() const noexcept override { return "FindDiPath"; }
    virtual std::string getShortName() const noexcept override { return "dipath"; }

    // ValueComputingAlgorithm interface
public:
    virtual bool deliver() override;

private:
    bool constructVertexPath;
    bool constructArcPath;
    Vertex *from;
    Vertex *to;
    bool twoWaySearch;
    unsigned long twoWayStepSize;
    bool fastPropertyMap;

    std::vector<Vertex*> vertexPath;
    std::vector<Arc*> arcPath;
    bool pathFound;

    DiGraph::size_type pr_num_vertices_seen;

    void runOneWaySearch();
    void runOneWayPathSearch();
    void runTwoWaySearch();
    void runTwoWayPathSearch();

    void constructVertexFromArcPath();

    // DiGraphAlgorithm interface
private:
    virtual void onDiGraphSet() override;
};

}

#include "finddipathalgorithm.cpp"

#endif // FINDDIPATHALGORITHM_H
