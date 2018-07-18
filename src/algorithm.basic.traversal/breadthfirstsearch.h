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

#include "graphtraversal.h"
#include "graph/digraph.h"
#include "graph/graph_functional.h"
#include "property/propertymap.h"

#include <boost/circular_buffer.hpp>

namespace Algora {

class Vertex;

template <template<typename T> class ModifiablePropertyType = PropertyMap>
class BreadthFirstSearch : public GraphTraversal<int>
{
public:
    explicit BreadthFirstSearch(bool computeValues = true, bool computeOrder = true)
        : GraphTraversal<int>(computeValues),
          computeOrder(computeOrder), maxBfsNumber(-1), maxLevel(-1),
          treeArc(arcNothing), nonTreeArc(arcNothing)
    { }

    virtual ~BreadthFirstSearch() { }

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

    void orderAsValues(bool order) {
        computeOrder = order;
    }

    // GraphTraversal interface
    unsigned int numVerticesReached() const {
        return getMaxBfsNumber() + 1;
    }

    // DiGraphAlgorithm interface
public:
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

        auto mapArcs = [&](const Vertex *v, const ArcMapping &avFun, const ArcPredicate &breakCondition) {
            diGraph->mapOutgoingArcsUntil(v, avFun, breakCondition);
            diGraph->mapIncomingArcsUntil(v, avFun, breakCondition);
        };
        auto mapOutgoingArcs = [&](const Vertex *v, const ArcMapping &avFun, const ArcPredicate &breakCondition) {
            diGraph->mapOutgoingArcsUntil(v, avFun, breakCondition);
        };
        auto mapIncomingArcs = [&](const Vertex *v, const ArcMapping &avFun, const ArcPredicate &breakCondition) {
            diGraph->mapIncomingArcsUntil(v, avFun, breakCondition);
        };

        auto mapArcsUntil = std::function<void(const Vertex *, const ArcMapping&, const ArcPredicate&)>(mapOutgoingArcs);
        if (onUndirectedGraph) {
            mapArcsUntil = mapArcs;
        } else if (onReverseGraph) {
            mapArcsUntil = mapIncomingArcs;
        }

        auto getTail = [](const Arc *a, const Vertex *) { return a->getTail(); };
        auto getHead = [](const Arc *a, const Vertex *) { return a->getHead(); };
        auto getOtherEndVertex = [](const Arc *a, const Vertex *v) { auto t = a->getTail(); return v == t ? a->getHead() : t; };
        auto getPeer = onUndirectedGraph ? getOtherEndVertex : (onReverseGraph ? getTail : getHead);

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

            mapArcsUntil(curr, [&](Arc *a) {
                    bool consider = onArcDiscovered(a);
                    stop |= arcStopCondition(a);
                    if (stop || !consider) {
                        return;
                    }
                    Vertex *peer = getPeer(a, curr);
                    if (!discovered(peer)) {
                        maxBfsNumber++;
                        if (computePropertyValues) {
                            int v = computeOrder ? maxBfsNumber : property->getValue(curr) + 1;
                            property->setValue(peer, v);
                        }
                        treeArc(a);
                        if (!onVertexDiscovered(peer)) {
                            return;
                        }

                        queue.push_back(peer);
                        discovered.setValue(peer, true);
                    } else {
                        nonTreeArc(a);
                    }
                }, [&](const Arc *) { return stop; });
        }
    }
    virtual std::string getName() const noexcept override { return "BFS"; }
    virtual std::string getShortName() const noexcept override { return "bfs"; }

    // ValueComputingAlgorithm interface
public:
    virtual unsigned int deliver() override
    {
        return maxBfsNumber + 1;
    }

private:
    bool computeOrder;
    int maxBfsNumber;
    int maxLevel;

    ArcMapping treeArc;
    ArcMapping nonTreeArc;

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
