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

#include "basic_algorithms.h"

#include "graph/digraph.h"
#include "property/propertymap.h"

namespace Algora {

bool hasDiPath(DiGraph *diGraph, Vertex *from, Vertex *to) {
    FindDiPathAlgorithm findDiPath(false);
    return runDiPathAlgorithm(diGraph, from, to, findDiPath);
}

bool isAcyclic(DiGraph *diGraph) {
    TopSortAlgorithm topSort(false);
    int sortedVertices = runAlgorithm(topSort, diGraph);
    return sortedVertices == (int) diGraph->getSize();
}

bool isStronglyConnected(DiGraph *diGraph)
{
    return countStrongComponents(diGraph) == 1;
}

int countStrongComponents(DiGraph *diGraph)
{
    TarjanSCCAlgorithm tarjan;
    PropertyMap<int> sccs(-1);
    tarjan.usePropertyMap(&sccs);
    return runAlgorithm(tarjan, diGraph);
}

bool isBiconnected(DiGraph *diGraph)
{
    return countBiconnectedComponents(diGraph) == 1;
}

int countBiconnectedComponents(DiGraph *diGraph)
{
    BiconnectedComponentsAlgorithm bic;
    std::vector<int> empty;
    PropertyMap<std::vector<int> > bics(empty);
    bic.usePropertyMap(&bics);
    return runAlgorithm(bic, diGraph);
}

bool runDiPathAlgorithm(DiGraph *diGraph, Vertex *from, Vertex *to, FindDiPathAlgorithm &a)
{
    a.setSourceVertex(from);
    a.setTargetVertex(to);
    return runAlgorithm(a, diGraph);
}

}


