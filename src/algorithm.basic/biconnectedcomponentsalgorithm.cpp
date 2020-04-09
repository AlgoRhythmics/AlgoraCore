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

#include "biconnectedcomponentsalgorithm.h"
#include "algorithm.basic.traversal/depthfirstsearch.h"

#include "graph/digraph.h"
#include "property/propertymap.h"

//#define DEBUG_BIC

#ifdef DEBUG_BIC
#include <iostream>
#define PRINT_DEBUG(msg) std::cout << msg << std::endl;
#define IF_DEBUG(cmd) cmd;
#else
#define PRINT_DEBUG(msg)
#define IF_DEBUG(cmd)
#endif

namespace Algora {

int findBiconnectedComponents(std::vector<Vertex *> &dfsOrderRev,
                              PropertyMap<DFSResult> &dfs, ModifiableProperty<std::vector<int> > &bics);

BiconnectedComponentsAlgorithm::BiconnectedComponentsAlgorithm()
    : numBics(0)
{

}

BiconnectedComponentsAlgorithm::~BiconnectedComponentsAlgorithm()
{
}

void BiconnectedComponentsAlgorithm::run()
{
    DFSResult none;
    PropertyMap<DFSResult> dfsResult(none);
    DepthFirstSearch<PropertyMap, false, true> dfs;
    dfs.useModifiableProperty(&dfsResult);

    auto n = diGraph->getSize();
    std::vector<Vertex*> dfsOrderRev(n, nullptr);
    int verticesReached = 0;
    diGraph->mapVertices([&](Vertex *v) {
        auto dfsNum = dfsResult(v).dfsNumber;
        if (dfsNum == -1) {
            PRINT_DEBUG("Running DFS starting from " << v);
            dfs.setStartVertex(v);
            verticesReached += runAlgorithm(dfs, diGraph);
        }
        dfsNum = dfsResult(v).dfsNumber;
        dfsOrderRev[verticesReached - 1 - dfsNum] = v;
    });
    numBics = findBiconnectedComponents(dfsOrderRev, dfsResult, *property);
}

int BiconnectedComponentsAlgorithm::deliver()
{
    return numBics;
}

int findBiconnectedComponents(std::vector<Vertex*> &dfsOrderRev,
                              PropertyMap<DFSResult> &dfs, ModifiableProperty<std::vector<int> > &bics) {
    // cut(v) <=> (v, u) in DfsTree : lowNumber(u) >= dfsNumber(v), if v not root
    //            v has > 1 child in DfsTree, if v is root
    int curBic = 0;

    for (Vertex *u : dfsOrderRev) {
        PRINT_DEBUG("Considering vertex " << u << " with number "
                    << dfs(u).dfsNumber  << ", low " << dfs(u).lowNumber  << ", parent " << dfs(u).parent);
        const Vertex *v = dfs(u).parent;
        if (!v) {
            // u is root
            PRINT_DEBUG(u << " is root.")
            // BUGFIX: If u is isolated, it forms a separate BIC
            if (bics(u).empty()) {
                bics[u].push_back(curBic);
                curBic++;
            }
            continue;
        }
        bics[u].push_back(curBic);
        PRINT_DEBUG(u << " belongs to BIC " << curBic << ".")
        // is v cut vertex or root?
        if (dfs(u).lowNumber >= dfs(v).dfsNumber) {
            // v is a cut vertex or root and u is its child
            PRINT_DEBUG(v << " belongs to BIC " << curBic << " and is cut vertex or root.")
            bics[v].push_back(curBic);
            curBic++;
        }
    }
    return curBic;
}

}
