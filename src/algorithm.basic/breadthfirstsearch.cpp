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


#include "breadthfirstsearch.h"

#include "graph/digraph.h"
#include "graph/vertex.h"
#include "graph/arc.h"
#include "property/propertymap.h"

#include <deque>

namespace Algora {

BreadthFirstSearch::BreadthFirstSearch()
    : startVertex(0), maxBfsNumber(-1)
{

}

BreadthFirstSearch::~BreadthFirstSearch()
{

}

bool BreadthFirstSearch::prepare()
{
    return PropertyComputingAlgorithm<bool, int>::prepare()
            && ( startVertex == 0 || diGraph->containsVertex(startVertex));
}

void BreadthFirstSearch::run()
{
    if (startVertex == 0) {
        startVertex = diGraph->getAnyVertex();
    }

    std::deque<Vertex*> queue;
    PropertyMap<bool> discovered(false);
    PropertyMap<int> &bfsNumber = *propertyMap;

    maxBfsNumber = 0;
    queue.push_back(startVertex);
    discovered[startVertex] = true;
    bfsNumber[startVertex] = maxBfsNumber;

    while (!queue.empty()) {
        Vertex *curr = queue.front();
        queue.pop_front();

        diGraph->visitOutgoingArcs(curr, [&](Arc *a) {
            Vertex *head = a->getHead();
            if (!discovered(head)) {
                queue.push_back(a->getHead());
                discovered[head] = true;
                maxBfsNumber++;
                bfsNumber[head] = maxBfsNumber;
            }
        });
    }
}

bool BreadthFirstSearch::deliver()
{
    return maxBfsNumber + 1 == diGraph->getSize();
}

void BreadthFirstSearch::onDiGraphSet()
{
    maxBfsNumber = -1;
}

}
