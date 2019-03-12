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

#include "eccentricityalgorithm.h"
#include "algorithm.basic.traversal/breadthfirstsearch.h"

#include "graph/digraph.h"
#include "algorithm/digraphalgorithmexception.h"

#include <climits>

namespace Algora {

const int EccentricityAlgorithm::INFINITE = INT_MAX;

EccentricityAlgorithm::EccentricityAlgorithm()
    : ValueComputingAlgorithm<int>(), vertex(nullptr), eccentricity(INFINITE)
{

}

bool EccentricityAlgorithm::prepare()
{
    return ValueComputingAlgorithm<int>::prepare() && diGraph->containsVertex(vertex);
}

void EccentricityAlgorithm::run()
{
    BreadthFirstSearch<> bfs(false);
    bfs.setStartVertex(vertex);
    bfs.orderAsValues(false);
    eccentricity = runAlgorithm(bfs, diGraph) == diGraph->getSize() ? bfs.getMaxLevel() :  INFINITE;
}

void EccentricityAlgorithm::onDiGraphSet() {
    ValueComputingAlgorithm<int>::onDiGraphSet();
    eccentricity = -1;
}

}
