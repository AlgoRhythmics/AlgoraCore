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

#ifndef BREADTHFIRSTSEARCH_H
#define BREADTHFIRSTSEARCH_H

#include "algorithm/propertycomputingalgorithm.h"
#include "graph/graph_functional.h"

namespace Algora {

class Vertex;

class BreadthFirstSearch : public PropertyComputingAlgorithm<bool, int>
{
public:
    explicit BreadthFirstSearch(bool computeValues = true);
    virtual ~BreadthFirstSearch();

    void setStartVertex(const Vertex *v) {
        startVertex = v;
    }

    void onVertexDiscover(const VertexPredicate &vFun) {
        onVertexDiscovered = vFun;
    }

    void onArcDiscover(const ArcPredicate &aFun) {
        onArcDiscovered = aFun;
    }

    void setVertexStopCondition(const VertexPredicate &vStop) {
        vertexStopCondition = vStop;
    }

    void setArcStopCondition(const ArcPredicate &aStop) {
        arcStopCondition = aStop;
    }


    int getMaxBfsNumber() const {
        return maxBfsNumber;
    }

    void reverseArcDirection(bool reverse) {
        useReversedArc = reverse;
    }

    // DiGraphAlgorithm interface
public:
    virtual bool prepare() override;
    virtual void run() override;
    virtual std::string getName() const noexcept override { return "BFS"; }
    virtual std::string getShortName() const noexcept override { return "bfs"; }

    // ValueComputingAlgorithm interface
public:
    virtual bool deliver() override;

private:
    const Vertex *startVertex;
    int maxBfsNumber;

    VertexPredicate onVertexDiscovered;
    ArcPredicate onArcDiscovered;
    VertexPredicate vertexStopCondition;
    ArcPredicate arcStopCondition;
    bool useReversedArc;

    // DiGraphAlgorithm interface
private:
    virtual void onDiGraphSet() override;
};

}

#endif // BREADTHFIRSTSEARCH_H
