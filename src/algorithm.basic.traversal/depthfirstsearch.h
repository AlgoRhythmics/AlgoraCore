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

#ifndef DEPTHFIRSTSEARCH_H
#define DEPTHFIRSTSEARCH_H

//#define DEBUG_DFS

#ifdef DEBUG_DFS
#include <iostream>
#define PRINT_DEBUG(msg) std::cout << msg << std::endl;
#define IF_DEBUG(cmd) cmd;
#else
#define PRINT_DEBUG(msg)
#define IF_DEBUG(cmd)
#endif

#include "graphtraversal.h"
#include "graph/digraph.h"
#include "property/propertymap.h"
#include "graph/graph_functional.h"
#include <limits>

namespace Algora {

class Vertex;

struct DFSResult {
    long long dfsNumber = -1;
    long long lowNumber = -1;
    const Vertex *parent = nullptr;
    explicit DFSResult(long long dfs=-1, long long low=-1, Vertex *p=nullptr) :
        dfsNumber(dfs), lowNumber(low), parent(p) {}
    DFSResult(const Vertex *p) : parent(p) {}
};

template <template<typename T> class ModifiablePropertyType = PropertyMap,
          bool reverseArcDirection = false, bool ignoreArcDirection = false>
class DepthFirstSearch : public GraphTraversal<DFSResult, reverseArcDirection, ignoreArcDirection>
{
public:
    constexpr static DiGraph::size_type INF = std::numeric_limits<DiGraph::size_type>::max();

    DepthFirstSearch(bool computeValues = true)
        : GraphTraversal<DFSResult,reverseArcDirection,ignoreArcDirection>(computeValues),
          verticesReached(INF),
          treeArc(arcNothing), nonTreeArc(arcNothing)
    {
        discovered.setDefaultValue(false);
    }

    virtual ~DepthFirstSearch() { }

    void onTreeArcDiscover(const ArcMapping &aFun) {
        treeArc = aFun;
    }

    void onNonTreeArcDiscover(const ArcMapping &aFun) {
        nonTreeArc = aFun;
    }


    // GraphTraversal interface
		DiGraph::size_type numVerticesReached() const override {
        return verticesReached;
    }

    // DiGraphAlgorithm interface
public:
    virtual void run() override
    {
        const Vertex *source =
                this->startVertex != nullptr ? this->startVertex : this->diGraph->getAnyVertex();

				DiGraph::size_type nextDepth = 0;
        bool stop = false;
        discovered.resetAll();
        dfs(source, nextDepth, stop);
        verticesReached = nextDepth;
    }

    virtual std::string getName() const noexcept override { return "DFS"; }
    virtual std::string getShortName() const noexcept override { return "dfs"; }

    bool vertexDiscovered(const Vertex *v) {
        return discovered(v);
    }

    // ValueComputingAlgorithm interface
public:
    virtual DiGraph::size_type deliver() override
    {
        return verticesReached;
    }

private:
    DiGraph::size_type verticesReached;
    ArcMapping treeArc;
    ArcMapping nonTreeArc;
    ModifiablePropertyType<bool> discovered;

    void dfs(const Vertex *v, DiGraph::size_type &depth, bool &stop) {
        discovered[v] = true;
        DFSResult *cur = nullptr;
        if (this->computePropertyValues) {
            cur = &(*this->property)[v];
            cur->dfsNumber = depth;
            cur->lowNumber = depth;
        }
        depth++;
        PRINT_DEBUG(v << " : low = " << cur->lowNumber);

        if (!this->onVertexDiscovered(v)) {
            return;
        }

        stop |= this->vertexStopCondition(v);

        if (stop) {
            return;
        }

        auto vm = [&](const Vertex *v, const Vertex *u, Arc *arc) {
            PRINT_DEBUG("Considering child " << u << " of " << v);

            bool consider = this->onArcDiscovered(arc);
            stop |= this->arcStopCondition(arc);

            if (stop || !consider) {
                return;
            }

            if (!discovered[u]) {
                if (this->computePropertyValues) {
                    (*this->property)[u].parent = v;
                }
                PRINT_DEBUG("Set parent of " << u << " to " << (*property)[u].parent);
                treeArc(arc);

                dfs(u, depth, stop);

                if (stop) {
                    return;
                }

                if (this->computePropertyValues) {
                    if ((*this->property)[u].lowNumber < cur->lowNumber) {
                        PRINT_DEBUG("Updating low from " << cur->lowNumber << " to " << (*property)[u].lowNumber);
                        cur->lowNumber = (*this->property)[u].lowNumber;
                        PRINT_DEBUG("Low is now " << (*property)[v].lowNumber)
                    }
                }
            } else {
                nonTreeArc(arc);
                if (this->computePropertyValues && cur->parent != u && (*this->property)[u].dfsNumber < cur->lowNumber) {
                    PRINT_DEBUG("Updating low from " << cur->lowNumber << " to " << (*property)[u].dfsNumber);
                    cur->lowNumber = (*this->property)[u].dfsNumber;
                    PRINT_DEBUG("Low is now " << (*property)[v].lowNumber);
                }
            }
        };
        if (!stop && (ignoreArcDirection || !reverseArcDirection)) {
            this->diGraph->mapOutgoingArcsUntil(v, [&](Arc *a) { vm(v, a->getHead(), a); }, [&stop](const Arc *) { return stop; });
        }
        if (!stop && (ignoreArcDirection || reverseArcDirection)) {
            this->diGraph->mapIncomingArcsUntil(v, [&](Arc *a) { vm(v, a->getTail(), a); }, [&stop](const Arc *) { return stop; });
        }
    }
};

}

#endif // DEPTHFIRSTSEARCH_H
