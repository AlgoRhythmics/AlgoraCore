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

#include "breadthfirstsearch.h"

#include "graph/digraph.h"
#include "graph/vertex.h"
#include "graph/arc.h"
#include "graph/reversearc.h"
#include "property/propertymap.h"

#include <deque>

namespace Algora {

BreadthFirstSearch::BreadthFirstSearch(bool computeValues)
    : PropertyComputingAlgorithm<bool, int>(computeValues),
      startVertex(0), maxBfsNumber(-1),
      onVertexDiscovered(vertexTrue), onArcDiscovered(arcTrue),
      vertexStopCondition(vertexFalse), arcStopCondition(arcFalse),
      useReversedArc(false)
{

}

BreadthFirstSearch::~BreadthFirstSearch()
{

}

bool BreadthFirstSearch::prepare()
{
    return PropertyComputingAlgorithm<bool, int>::prepare()
            && ( startVertex == 0 || (diGraph->containsVertex(startVertex) && startVertex->isValid()));
}

void BreadthFirstSearch::run()
{
    if (startVertex == 0) {
        startVertex = diGraph->getAnyVertex();
    }

    std::deque<const Vertex*> queue;
    PropertyMap<bool> discovered(false);
    PropertyMap<int> *bfsNumber = propertyMap;

    maxBfsNumber = 0;
    queue.push_back(startVertex);
    discovered[startVertex] = true;
    if (computePropertyValues) {
        (*bfsNumber)[startVertex] = maxBfsNumber;
    }

    bool stop = false;

    while (!stop && !queue.empty()) {
        const Vertex *curr = queue.front();
        queue.pop_front();
        stop |= vertexStopCondition(curr);
        if (stop) {
            break;
        }

        if (useReversedArc) {
            diGraph->mapIncomingArcsUntil(curr, [&](Arc *a) {
                bool consider = onArcDiscovered(a);
                stop |= arcStopCondition(a);
                if (stop || !consider) {
                    return;
                }
                Vertex *tail= a->getTail();
                if (!discovered(tail)) {
                    treeArc(a);
                    if (!onVertexDiscovered(tail)) {
                        return;
                    }

                    queue.push_back(tail);
                    discovered[tail] = true;
                    maxBfsNumber++;
                    if (computePropertyValues) {
                        (*bfsNumber)[tail] = maxBfsNumber;
                    }
                } else {
                    nonTreeArc(a);
                }
            }, [&](const Arc *) { return stop; });
        } else {
            diGraph->mapOutgoingArcsUntil(curr, [&](Arc *a) {
                bool consider = onArcDiscovered(a);
                stop |= arcStopCondition(a);
                if (stop || !consider) {
                    return;
                }
                Vertex *head = a->getHead();
                if (!discovered(head)) {
                    treeArc(a);
                    if (!onVertexDiscovered(head)) {
                        return;
                    }

                    queue.push_back(head);
                    discovered[head] = true;
                    maxBfsNumber++;
                    if (computePropertyValues) {
                        (*bfsNumber)[head] = maxBfsNumber;
                    }
                } else {
                    nonTreeArc(a);
                }
            }, [&](const Arc *) { return stop; });
        }
    }
}

bool BreadthFirstSearch::deliver()
{
    return maxBfsNumber + 1 == (int) diGraph->getSize();
}

void BreadthFirstSearch::onDiGraphSet()
{
    maxBfsNumber = -1;
}

}
