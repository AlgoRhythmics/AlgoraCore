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


#include "topsortalgorithm.h"

#include "graph/digraph.h"
#include "graph/vertex.h"
#include "graph/arc.h"
#include "property/propertymap.h"

#include <deque>

namespace Algora {

TopSortAlgorithm::TopSortAlgorithm(bool computeValues)
    : PropertyComputingAlgorithm(computeValues)
{

}

TopSortAlgorithm::~TopSortAlgorithm()
{

}

void TopSortAlgorithm::run()
{
    sequence.clear();

    PropertyMap<int> inDegree(-1);

    std::deque<Vertex*> queue;

    diGraph->mapVertices([&](Vertex *v) {
        int indegree = diGraph->getInDegree(v);
        if (indegree == 0) {
            queue.push_back(v);
        } else {
            inDegree[v] = indegree;
        }
    });

    int ts = 0;
    while (!queue.empty()) {
        Vertex *v = queue.front();
        queue.pop_front();

        if (computePropertyValues) {
            (*propertyMap)[v] = ts;
            ts++;
        }

        sequence.push_back(v);

        diGraph->mapOutgoingArcs(v, [&](Arc *a) {
            Vertex *head = a->getHead();
            if (inDegree(head) == 1) {
                queue.push_back(head);
            } else {
                inDegree[head]--;
            }
        });
    }
}

}
