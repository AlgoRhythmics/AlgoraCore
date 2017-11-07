/**
 * Copyright (C) 2013 - 2017 : Kathrin Hanauer
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

#ifndef DEPTHFIRSTSEARCH_H
#define DEPTHFIRSTSEARCH_H

#include "algorithm/propertycomputingalgorithm.h"

namespace Algora {

class Vertex;

struct DFSResult {
    int dfsNumber = -1;
    int lowNumber = -1;
    Vertex *parent = 0;
    explicit DFSResult(int dfs=-1, int low=-1, Vertex *p=0) :
        dfsNumber(dfs), lowNumber(low), parent(p) {}
    DFSResult(Vertex *p) : parent(p) {}
};

class DepthFirstSearch : public PropertyComputingAlgorithm<int, DFSResult>
{
public:
    DepthFirstSearch();
    virtual ~DepthFirstSearch();

    void setStartVertex(Vertex *v) {
        startVertex = v;
    }

    void setIgnoreArcDirections(bool ignore) {
       ignoreArcDirections = ignore;
    }

    // DiGraphAlgorithm interface
public:
    virtual bool prepare() override;
    virtual void run() override;
    virtual std::string getName() const noexcept override { return "DFS"; }
    virtual std::string getShortName() const noexcept override { return "dfs"; }

    // ValueComputingAlgorithm interface
public:
    virtual int deliver() override;

private:
    Vertex *startVertex;
    int maxDfsNumber;
    bool ignoreArcDirections;
};

}

#endif // DEPTHFIRSTSEARCH_H
