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
#include <climits>

namespace Algora {

class Vertex;

template <template<typename T> class ModifiablePropertyType = PropertyMap,
          bool valueComputation = true>
class BreadthFirstSearch : public GraphTraversal<unsigned long long>
{
public:
    static constexpr unsigned long long INF = ULLONG_MAX;

    explicit BreadthFirstSearch(bool computeValues = true, bool computeOrder = true)
        : GraphTraversal<unsigned long long>(valueComputation && computeValues),
          computeOrder(computeOrder), maxBfsNumber(-1), maxLevel(-1),
          treeArc(arcNothing), nonTreeArc(arcNothing)
    {
        discovered.setDefaultValue(false);
    }

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

    void levelAsValues(bool levels) {
        computeOrder = !levels;
    }

    bool vertexDiscovered(const Vertex *v) {
        return discovered(v);
    }

    // GraphTraversal interface
    unsigned long long numVerticesReached() const override {
        return getMaxBfsNumber() + 1;
    }

    // DiGraphAlgorithm interface
public:
    virtual void run() override
    {
        if (startVertex == 0) {
            startVertex = diGraph->getAnyVertex();
        }


        maxBfsNumber = 0ULL;
        maxLevel = 0ULL;

        //queue.clear();
        boost::circular_buffer<const Vertex*> queue;
        queue.set_capacity(diGraph->getSize());
        discovered.resetAll();

        queue.push_back(startVertex);
        queue.push_back(nullptr);
        discovered.setValue(startVertex, true);
        if (valueComputation && computePropertyValues) {
            property->setValue(startVertex, 0);
        }

        bool stop = !onVertexDiscovered(startVertex);

        auto mapArcs = [this](const Vertex *v, const ArcMapping &avFun, const ArcPredicate &breakCondition) {
            diGraph->mapOutgoingArcsUntil(v, avFun, breakCondition);
            diGraph->mapIncomingArcsUntil(v, avFun, breakCondition);
        };
        auto mapOutgoingArcs = [this](const Vertex *v, const ArcMapping &avFun, const ArcPredicate &breakCondition) {
            diGraph->mapOutgoingArcsUntil(v, avFun, breakCondition);
        };
        auto mapIncomingArcs = [this](const Vertex *v, const ArcMapping &avFun, const ArcPredicate &breakCondition) {
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
        const auto &getPeer = onUndirectedGraph ? getOtherEndVertex : (onReverseGraph ? getTail : getHead);

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

            mapArcsUntil(curr, [this,curr,&stop,&queue,&getPeer](Arc *a) {
                    bool consider = onArcDiscovered(a);
                    stop |= arcStopCondition(a);
                    if (stop || !consider) {
                        return;
                    }
                    Vertex *peer = getPeer(a, curr);
                    if (!discovered(peer)) {
                        maxBfsNumber++;
                        if (valueComputation && computePropertyValues) {
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
                }, [&stop](const Arc *) { return stop; });
        }
    }
    virtual std::string getName() const noexcept override { return "BFS"; }
    virtual std::string getShortName() const noexcept override { return "bfs"; }

    // ValueComputingAlgorithm interface
public:
    virtual unsigned long long deliver() override
    {
        return maxBfsNumber + 1ULL;
    }

private:
    bool computeOrder;
    unsigned long long maxBfsNumber;
    unsigned long long maxLevel;

    ArcMapping treeArc;
    ArcMapping nonTreeArc;

    // DiGraphAlgorithm interface
private:
    virtual void onDiGraphSet() override
    {
        maxBfsNumber = INF;
        maxLevel = INF;
    }
    ModifiablePropertyType<bool> discovered;
};

}

#endif // BREADTHFIRSTSEARCH_H
