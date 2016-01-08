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


#include "finddipathalgorithm.h"

#include "graph/digraph.h"
#include "graph/vertex.h"
#include "graph/arc.h"
#include "property/propertymap.h"
#include <deque>
#include <algorithm>

namespace Algora {

FindDiPathAlgorithm::FindDiPathAlgorithm(bool constructPath)
    : constructPath(constructPath), from(0), to(0)
{

}

FindDiPathAlgorithm::~FindDiPathAlgorithm()
{

}

bool FindDiPathAlgorithm::prepare()
{
    return ValueComputingAlgorithm<bool>::prepare()
            && from != 0
            && to != 0
            && diGraph->containsVertex(from)
            && diGraph->containsVertex(to);
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

    std::deque<Vertex*> queue;
    PropertyMap<bool> discovered(false);
    PropertyMap<Arc*> pred(0);

    queue.push_back(from);
    discovered[from] = true;

    while (!pathFound && !queue.empty()) {
        Vertex *curr = queue.front();
        queue.pop_front();

        diGraph->visitOutgoingArcs(curr, [&](Arc *a) {
            Vertex *head = a->getHead();
            if (!discovered(head)) {
                queue.push_back(a->getHead());
                if (constructPath) {
                    pred[head] = a;
                }
                discovered[head] = true;
                if (head == to) {
                    pathFound = true;
                }
            }
        });
    }

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
