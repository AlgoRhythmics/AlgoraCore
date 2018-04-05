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

#include "finddipathalgorithm.h"

#include "graph/digraph.h"
#include "graph/vertex.h"
#include "graph/arc.h"
#include "property/propertymap.h"
#include "algorithm.basic/breadthfirstsearch.h"
#include "algorithm/digraphalgorithmexception.h"

#include <algorithm>

namespace Algora {

FindDiPathAlgorithm::FindDiPathAlgorithm(bool constructPath)
    : constructPath(constructPath), from(0), to(0), pathFound(false)
{

}

FindDiPathAlgorithm::~FindDiPathAlgorithm()
{

}

bool FindDiPathAlgorithm::prepare()
{
    bool ok = ValueComputingAlgorithm<bool>::prepare()
            && from != 0
            && to != 0
            && diGraph->containsVertex(from)
            && diGraph->containsVertex(to);
    return ok;
}

void FindDiPathAlgorithm::run()
{
    vertexPath.clear();
    arcPath.clear();
    pathFound = false;

    if (from == to) {
        pathFound = true;
        return;
    }


    BreadthFirstSearch bfs(false);
    bfs.setGraph(diGraph);
    PropertyMap<Arc*> pred(0);

    bfs.setStartVertex(from);
    bfs.setVertexStopCondition([&](const Vertex *) { return pathFound; });
    bfs.onArcDiscover([&](const Arc *a) {
        Arc *arc = const_cast<Arc*>(a);
        Vertex *head = a->getHead();
        if(!pred[head]) {
            pred[head] = arc;
        }
        if (head == to) {
            pathFound = true;
        }
        return true;
    });

    if (!bfs.prepare()){
       throw DiGraphAlgorithmException(this, "Could not prepare BFS algorithm.");
    }
    bfs.run();
    bfs.deliver();

    if (pathFound && constructPath) {
        Vertex *p = to;
        while (p != from) {
            Arc *a = pred(p);
            vertexPath.push_back(p);
            arcPath.push_back(a);
            p = a->getTail();
        }
        vertexPath.push_back(from);
        std::reverse(vertexPath.begin(), vertexPath.end());
        std::reverse(arcPath.begin(), arcPath.end());
    }
}

bool FindDiPathAlgorithm::deliver()
{
    return pathFound;
}

void Algora::FindDiPathAlgorithm::onDiGraphSet()
{
    vertexPath.clear();
    arcPath.clear();
    pathFound = false;
}

}
