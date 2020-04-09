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

#ifndef BREADTHFIRSTSEARCH_H
#define BREADTHFIRSTSEARCH_H

#include "graphtraversal.h"
#include "graph/digraph.h"
#include "graph/graph_functional.h"
#include "property/propertymap.h"

#include <boost/circular_buffer.hpp>
#include <limits>

namespace Algora {

class Vertex;

template <template<typename T> class ModifiablePropertyType = PropertyMap,
          bool valueComputation = true,
          bool reverseArcDirection = false, bool ignoreArcDirection = false>
class BreadthFirstSearch : public GraphTraversal<DiGraph::size_type,
        reverseArcDirection, ignoreArcDirection>
{
public:
    static constexpr DiGraph::size_type INF = std::numeric_limits<DiGraph::size_type>::max();

    explicit BreadthFirstSearch(bool computeValues = true, bool computeOrder = true)
        : GraphTraversal<DiGraph::size_type,reverseArcDirection, ignoreArcDirection>(
              valueComputation && computeValues),
          computeOrder(computeOrder), maxBfsNumber(INF), maxLevel(INF),
          treeArc(arcNothing), nonTreeArc(arcNothing),
          stopAfterEachNeighborsScan(false)
    {
        discovered.setDefaultValue(false);
    }

    virtual ~BreadthFirstSearch() { }

    void setStartVertices(const std::vector<const Vertex*> &startVertices) {
        this->startVertices = startVertices;
    }

    void setStartVertices(const std::vector<const Vertex*> &&startVertices) {
        this->startVertices = std::move(startVertices);
    }

    void autoStopAfterNeighborsScans(bool stop) {
        stopAfterEachNeighborsScan = stop;
    }

    void onTreeArcDiscover(const ArcMapping &aFun) {
        treeArc = aFun;
    }

    void onNonTreeArcDiscover(const ArcMapping &aFun) {
        nonTreeArc = aFun;
    }

    DiGraph::size_type getMaxBfsNumber() const {
        return maxBfsNumber;
    }

    DiGraph::size_type getMaxLevel() const {
        return maxLevel;
    }

    void orderAsValues(bool order) {
        computeOrder = order;
    }

    void levelAsValues(bool levels) {
        computeOrder = !levels;
    }

    bool vertexDiscovered(const Vertex *v) const {
        return discovered(v);
    }

    bool isExhausted() const {
        return exhausted;
    }

    // GraphTraversal interface
    DiGraph::size_type numVerticesReached() const override {
        auto n = getMaxBfsNumber();
        if (n == INF) {
            return 0ULL;
        }
        return n + 1;
    }

    // DiGraphAlgorithm interface
public:
    virtual bool prepare() override
    {
        return PropertyComputingAlgorithm<DiGraph::size_type, DiGraph::size_type>::prepare()
                && ( this->startVertex == nullptr
                     || (this->diGraph->containsVertex(this->startVertex) &&
                         this->startVertex->isValid()))
                && (startVertices.empty()
                    || std::all_of(startVertices.begin(), startVertices.end(),
                                   [this](const Vertex *v){
                        return this->diGraph->containsVertex(v) && v->isValid(); }));
    }

    virtual void run() override
    {
        if (this->startVertex == nullptr && startVertices.empty()) {
            this->startVertex = this->diGraph->getAnyVertex();
        }

        maxBfsNumber = 0ULL;
        maxLevel = 0ULL;

        queue.clear();
        queue.set_capacity(this->diGraph->getSize());
        discovered.resetAll();
        exhausted = false;

        if (startVertices.empty()) {
            if (!this->onVertexDiscovered(this->startVertex)) {
                return;
            }
            queue.push_back(this->startVertex);
            queue.push_back(nullptr);
            discovered.setValue(this->startVertex, true);
            if (valueComputation && this->computePropertyValues) {
                this->property->setValue(this->startVertex, 0);
            }
        } else {
            for (auto *v : startVertices) {
                if (!this->onVertexDiscovered(v)) {
                    continue;
                }
                queue.push_back(v);
                discovered.setValue(v, true);
                if (valueComputation && this->computePropertyValues) {
                    int c = computeOrder ? maxBfsNumber : 0;
                    this->property->setValue(v, c);
                }
                maxBfsNumber++;
            }
            if (queue.empty()) {
                return;
            }
            queue.push_back(nullptr);
            maxBfsNumber--;
        }
        resume();
    }

    virtual void resume()
    {
        auto getTail = [](const Arc *a, const Vertex *) { return a->getTail(); };
        auto getHead = [](const Arc *a, const Vertex *) { return a->getHead(); };
        auto getOtherEndVertex = [](const Arc *a, const Vertex *v) {
            auto t = a->getTail(); return v == t ? a->getHead() : t;
        };
        const auto &getPeer = ignoreArcDirection ? getOtherEndVertex
                                                : (reverseArcDirection ? getTail : getHead);
        bool stop = false;

        while (!stop && !this->queue.empty()) {
            const Vertex *curr = this->queue.front();

            if (curr) {
                stop |= this->vertexStopCondition(curr);
                if (stop) {
                    break;
                }
                this->queue.pop_front();
            } else {
                this->queue.pop_front();
                if (!this->queue.empty()) {
                    this->queue.push_back(nullptr);
                    this->maxLevel++;
                }
                continue;
            }

            auto arcMapping = [this,curr,&stop,&getPeer](Arc *a) {
                bool consider = this->onArcDiscovered(a);
                if (!consider) {
                    return;
                }
                stop |= this->arcStopCondition(a);
                if (stop) {
                    return;
                }
                Vertex *peer = getPeer(a, curr);
                if (!this->discovered(peer)) {
                    this->maxBfsNumber++;
                    if (valueComputation && this->computePropertyValues) {
                        int v = this->computeOrder
                                ? this->maxBfsNumber : this->property->getValue(curr) + 1;
                        this->property->setValue(peer, v);
                    }
                    this->discovered.setValue(peer, true);
                    this->treeArc(a);
                    if (!this->onVertexDiscovered(peer)) {
                        return;
                    }

                    this->queue.push_back(peer);
                } else {
                    this->nonTreeArc(a);
                }
            };
            auto arcStopCondition = [&stop](const Arc *) { return stop; };

            if (ignoreArcDirection) {
                this->diGraph->mapOutgoingArcsUntil(curr, arcMapping, arcStopCondition);
                this->diGraph->mapIncomingArcsUntil(curr, arcMapping, arcStopCondition);
            } else if (reverseArcDirection) {
                this->diGraph->mapIncomingArcsUntil(curr, arcMapping, arcStopCondition);
            } else {
                this->diGraph->mapOutgoingArcsUntil(curr, arcMapping, arcStopCondition);
            }

            if (stopAfterEachNeighborsScan) {
                stop = true;
            }
        }
        if (!stop) {
            assert(this->queue.empty());
        }
        if (this->queue.empty()) {
            this->exhausted = true;
        }
    }
    virtual std::string getName() const noexcept override { return "BFS"; }
    virtual std::string getShortName() const noexcept override { return "bfs"; }

    // ValueComputingAlgorithm interface
public:
    virtual DiGraph::size_type deliver() override
    {
        return maxBfsNumber == INF ? INF : maxBfsNumber + 1ULL;
    }

private:
    bool computeOrder;
    DiGraph::size_type maxBfsNumber;
    DiGraph::size_type maxLevel;

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
    boost::circular_buffer<const Vertex*> queue;
    std::vector<const Vertex*> startVertices;
    bool exhausted;
    bool stopAfterEachNeighborsScan;
};

}

#endif // BREADTHFIRSTSEARCH_H
