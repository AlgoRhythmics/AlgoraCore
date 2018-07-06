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

#ifndef BREADTHFIRSTSEARCH_H
#define BREADTHFIRSTSEARCH_H

#include "algorithm/propertycomputingalgorithm.h"
#include "graph/digraph.h"
#include "graph/graph_functional.h"
#include "property/propertymap.h"

#include <boost/circular_buffer.hpp>

namespace Algora {

class Vertex;

template <template<typename T> class ModifiablePropertyType = PropertyMap>
class BreadthFirstSearch : public PropertyComputingAlgorithm<bool, int>
{
public:
    explicit BreadthFirstSearch(bool computeValues = true, bool computeOrder = true)
        : PropertyComputingAlgorithm<bool, int>(computeValues),
          startVertex(0), computeOrder(computeOrder), maxBfsNumber(-1), maxLevel(-1),
          onVertexDiscovered(vertexTrue), onArcDiscovered(arcTrue),
          vertexStopCondition(vertexFalse), arcStopCondition(arcFalse),
          treeArc(arcNothing), nonTreeArc(arcNothing),
          useReversedArc(false)
    {

    }
    virtual ~BreadthFirstSearch()
    {

    }

    void setStartVertex(const Vertex *v) {
        startVertex = v;
    }

    void onVertexDiscover(const VertexPredicate &vFun) {
        onVertexDiscovered = vFun;
    }

    void onArcDiscover(const ArcPredicate &aFun) {
        onArcDiscovered = aFun;
    }

    void setVertexStopCondition(const VertexPredicate &vStop) {
        vertexStopCondition = vStop;
    }

    void setArcStopCondition(const ArcPredicate &aStop) {
        arcStopCondition = aStop;
    }

    void onTreeArcDiscover(const ArcMapping &aFun) {
        treeArc = aFun;
    }

    void onNonTreeArcDiscover(const ArcMapping &aFun) {
        nonTreeArc = aFun;
    }

    int getMaxBfsNumber() const {
        return maxBfsNumber;
    }

    int getMaxLevel() const {
        return maxLevel;
    }

    void reverseArcDirection(bool reverse) {
        useReversedArc = reverse;
    }

    void orderAsValues(bool order) {
        computeOrder = order;
    }

    // DiGraphAlgorithm interface
public:
    virtual bool prepare() override
    {
        return PropertyComputingAlgorithm<bool, int>::prepare()
                && ( startVertex == 0 || (diGraph->containsVertex(startVertex) && startVertex->isValid()));
    }
    virtual void run() override
    {
        if (startVertex == 0) {
            startVertex = diGraph->getAnyVertex();
        }

        boost::circular_buffer<const Vertex*> queue(diGraph->getSize());
        ModifiablePropertyType<bool> discovered(false);

        maxBfsNumber = 0;
        maxLevel = 0;

        queue.push_back(startVertex);
        queue.push_back(nullptr);
        discovered.setValue(startVertex, true);
        if (computePropertyValues) {
            property->setValue(startVertex, 0);
        }

        bool stop = false;

        while (!stop && !queue.empty()) {
            const Vertex *curr = queue.front();
            queue.pop_front();
            if (curr == nullptr) {
                if (!queue.empty()) {
                    queue.push_back(nullptr);
                    maxLevel++;
                }
                continue;
            }
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
                        maxBfsNumber++;
                        if (computePropertyValues) {
                            int v =computeOrder ? maxBfsNumber : property->getValue(a->getHead()) + 1;
                            property->setValue(tail, v);
                        }
                        treeArc(a);
                        if (!onVertexDiscovered(tail)) {
                            return;
                        }

                        queue.push_back(tail);
                        discovered.setValue(tail, true);
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
                        maxBfsNumber++;
                        if (computePropertyValues) {
                            int v =computeOrder ? maxBfsNumber : property->getValue(a->getTail()) + 1;
                            property->setValue(head, v);
                        }
                        treeArc(a);
                        if (!onVertexDiscovered(head)) {
                            return;
                        }

                        queue.push_back(head);
                        discovered.setValue(head, true);
                    } else {
                        nonTreeArc(a);
                    }
                }, [&](const Arc *) { return stop; });
            }
        }
    }
    virtual std::string getName() const noexcept override { return "BFS"; }
    virtual std::string getShortName() const noexcept override { return "bfs"; }

    // ValueComputingAlgorithm interface
public:
    virtual bool deliver() override
    {
        return maxBfsNumber + 1 == (int) diGraph->getSize();
    }

private:
    const Vertex *startVertex;
    bool computeOrder;
    int maxBfsNumber;
    int maxLevel;

    VertexPredicate onVertexDiscovered;
    ArcPredicate onArcDiscovered;
    VertexPredicate vertexStopCondition;
    ArcPredicate arcStopCondition;
    ArcMapping treeArc;
    ArcMapping nonTreeArc;
    bool useReversedArc;

    // DiGraphAlgorithm interface
private:
    virtual void onDiGraphSet() override
    {
        maxBfsNumber = -1;
        maxLevel = -1;
    }
};

}

#endif // BREADTHFIRSTSEARCH_H
