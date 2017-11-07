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

#include "depthfirstsearch.h"

#include "graph/digraph.h"

#include "property/propertymap.h"

#include <iostream>

namespace Algora {

void dfs(DiGraph *g, Vertex *v, int &depth, PropertyMap<DFSResult> &pm,
         PropertyMap<bool> &discovered, bool ignoreDirection);

DepthFirstSearch::DepthFirstSearch()
    : startVertex(0), maxDfsNumber(-1), ignoreArcDirections(false)
{

}

DepthFirstSearch::~DepthFirstSearch()
{

}

bool DepthFirstSearch::prepare()
{
    return PropertyComputingAlgorithm<int, DFSResult>::prepare()
            && (startVertex == 0 || diGraph->containsVertex(startVertex));
}

void DepthFirstSearch::run()
{
    if (startVertex == 0) {
        startVertex = diGraph->getAnyVertex();
    }

    int nextDepth = 0;
    PropertyMap<DFSResult> &pm = *propertyMap;
    PropertyMap<bool> discovered(false);
    dfs(diGraph, startVertex, nextDepth, pm, discovered, ignoreArcDirections);
    maxDfsNumber = nextDepth - 1;
}

int DepthFirstSearch::deliver()
{
    return maxDfsNumber + 1;
}

void dfs(DiGraph *g, Vertex *v, int &depth, PropertyMap<DFSResult> &pm,
         PropertyMap<bool> &discovered, bool ignoreDirection) {

    discovered[v] = true;
    DFSResult &cur = pm[v];
    cur.dfsNumber = depth;
    cur.lowNumber = depth;
    depth++;
    //std::cout << v << " : low = " << cur.lowNumber << std::endl;

    auto vm = [&](Vertex *v, Vertex *u) {
        //std::cout << "Considering child " << u << std::endl;
        if (!discovered(u)) {
            pm[u] = DFSResult(v);
            //std::cout << "Set parent of " << u << " to " << pm(u).parent << std::endl;
            dfs(g, u, depth, pm, discovered, ignoreDirection);
            if (pm(u).lowNumber < cur.lowNumber) {
                //std::cout << "Updating low from " << cur.lowNumber << " to " << pm(u).lowNumber << std::endl;
                cur.lowNumber = pm(u).lowNumber;
                //std::cout << "Low is now " << pm(v).lowNumber << std::endl;
            }
        } else if (cur.parent != u && pm(u).dfsNumber < cur.lowNumber) {
            //std::cout << "Updating low from " << cur.lowNumber << " to " << pm(u).dfsNumber << std::endl;
            cur.lowNumber = pm(u).dfsNumber;
            //std::cout << "Low is now " << pm(v).lowNumber << std::endl;
        }
    };
    g->mapOutgoingArcs(v, [&](Arc *a) { vm(v, a->getHead()); });
    if (ignoreDirection) {
        g->mapIncomingArcs(v, [&](Arc *a) { vm(v, a->getTail()); });
    }
}

}
