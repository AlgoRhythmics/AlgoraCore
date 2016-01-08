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


#ifndef BREADTHFIRSTSEARCH_H
#define BREADTHFIRSTSEARCH_H

#include "algorithm/propertycomputingalgorithm.h"

namespace Algora {

class Vertex;

class BreadthFirstSearch : public PropertyComputingAlgorithm<bool, int>
{
public:
    BreadthFirstSearch();
    virtual ~BreadthFirstSearch();

    void setStartVertex(Vertex *v) {
        startVertex = v;
    }

    int getMaxBfsNumber() const {
        return maxBfsNumber;
    }

    // DiGraphAlgorithm interface
public:
    virtual bool prepare() override;
    virtual void run() override;

    // ValueComputingAlgorithm interface
public:
    virtual bool deliver() override;

private:
    Vertex *startVertex;
    int maxBfsNumber;

    // DiGraphAlgorithm interface
private:
    virtual void onDiGraphSet() override;
};

}

#endif // BREADTHFIRSTSEARCH_H
