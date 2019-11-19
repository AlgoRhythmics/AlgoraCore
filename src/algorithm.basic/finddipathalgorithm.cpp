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

#include "finddipathalgorithm.h"

#include "graph/digraph.h"
#include "graph/vertex.h"
#include "graph/arc.h"
#include "property/propertymap.h"
#include "algorithm.basic.traversal/breadthfirstsearch.h"
#include "algorithm/digraphalgorithmexception.h"

#include "property/fastpropertymap.h"

#include <algorithm>

namespace Algora {

template<template <typename T> typename property_map_type>
FindDiPathAlgorithm<property_map_type>::FindDiPathAlgorithm(bool constructVertexPath, bool constructArcPath, bool twoWaySearch)
    : constructVertexPath(constructVertexPath), constructArcPath(constructArcPath),
      from(nullptr), to(nullptr), twoWaySearch(twoWaySearch), twoWayStepSize(5UL),
      pathFound(false)
{

}

template<template <typename T> typename property_map_type>
bool FindDiPathAlgorithm<property_map_type>::prepare()
{
    bool ok = ValueComputingAlgorithm<bool>::prepare()
            && from != nullptr
            && to != nullptr
            && diGraph->containsVertex(from)
            && diGraph->containsVertex(to);
    return ok;
}

template<template <typename T> typename property_map_type>
void FindDiPathAlgorithm<property_map_type>::run()
{
    vertexPath.clear();
    arcPath.clear();

    if (from == to) {
        pathFound = true;
        return;
    }

    if (diGraph->isSink(from) || diGraph->isSource(to)) {
        pathFound = false;
        return;
    }

    pathFound = false;

    if (twoWaySearch) {
        if (constructVertexPath || constructArcPath) {
            runTwoWayPathSearch();
        } else  {
            runTwoWaySearch();
        }
    } else {
        runOneWaySearch();
    }
}

template<template <typename T> typename property_map_type>
bool FindDiPathAlgorithm<property_map_type>::deliver()
{
    return pathFound;
}

template<template <typename T> typename property_map_type>
void FindDiPathAlgorithm<property_map_type>::runTwoWaySearch()
{
    pathFound = false;

    BreadthFirstSearch<property_map_type,false> forwardBfs(false, false);
    forwardBfs.setGraph(diGraph);
    forwardBfs.setStartVertex(from);

    BreadthFirstSearch<property_map_type,false> backwardBfs(false, false);
    backwardBfs.setGraph(diGraph);
    backwardBfs.reverseArcDirection(true);
    backwardBfs.setStartVertex(to);

    bool reachable = false;
    auto forwardStop = twoWayStepSize;
    auto backwardStop = twoWayStepSize;

    forwardBfs.setArcStopCondition([&backwardBfs,&reachable](const Arc *a) {
        if (backwardBfs.vertexDiscovered(a->getHead())) {
            reachable = true;
        }
        return reachable;
    });
    backwardBfs.setArcStopCondition([&forwardBfs,&reachable](const Arc *a) {
        if (forwardBfs.vertexDiscovered(a->getTail())) {
            reachable = true;
        }
        return reachable;
    });

    forwardBfs.setVertexStopCondition([&forwardBfs,&forwardStop,this](const Vertex *) {
        if (forwardBfs.getMaxBfsNumber() >= forwardStop) {
            forwardStop += twoWayStepSize;
            return true;
        }
        return false;
    });

    backwardBfs.setVertexStopCondition([&backwardBfs,&backwardStop,this](const Vertex *) {
        if (backwardBfs.getMaxBfsNumber() >= backwardStop) {
            backwardStop += twoWayStepSize;
            return true;
        }
        return false;
    });

    forwardBfs.prepare();
    backwardBfs.prepare();

    forwardBfs.run();
    backwardBfs.run();

    while (!reachable && !forwardBfs.isExhausted() && !backwardBfs.isExhausted()) {
        forwardBfs.resume();
        backwardBfs.resume();
    }

    pathFound = reachable;
}

template<template <typename T> typename property_map_type>
void FindDiPathAlgorithm<property_map_type>::onDiGraphSet()
{
    vertexPath.clear();
    arcPath.clear();
    pathFound = false;
}

template<template <typename T> typename property_map_type>
void FindDiPathAlgorithm<property_map_type>::runTwoWayPathSearch()
{
    BreadthFirstSearch<property_map_type,false> forwardBfs(false, false);
    forwardBfs.setGraph(diGraph);
    forwardBfs.setStartVertex(from);

    BreadthFirstSearch<property_map_type,false> backwardBfs(false, false);
    backwardBfs.setGraph(diGraph);
    backwardBfs.reverseArcDirection(true);
    backwardBfs.setStartVertex(to);

    auto forwardStop = twoWayStepSize;
    auto backwardStop = twoWayStepSize;
    property_map_type<Arc*> treeArc(nullptr);
    Arc *fbLink = nullptr;

    forwardBfs.setArcStopCondition([fbLink](const Arc *) {
        return fbLink != nullptr;
    });
    backwardBfs.setArcStopCondition([fbLink](const Arc *) {
        return fbLink != nullptr;
    });

    forwardBfs.setVertexStopCondition([&forwardBfs,&forwardStop,this](const Vertex *) {
        if (forwardBfs.getMaxBfsNumber() >= forwardStop) {
            forwardStop+= twoWayStepSize;
            return true;
        }
        return false;
    });

    backwardBfs.setVertexStopCondition([&backwardBfs,&backwardStop,this](const Vertex *) {
        if (backwardBfs.getMaxBfsNumber() >= backwardStop) {
            backwardStop+= twoWayStepSize;
            return true;
        }
        return false;
    });

    forwardBfs.onTreeArcDiscover([&backwardBfs,&treeArc,&fbLink](const Arc *a) {
        if (fbLink) {
            return;
        }
        auto head = a->getHead();
        if (backwardBfs.vertexDiscovered(head)) {
            fbLink = const_cast<Arc*>(a);
        } else {
            treeArc[head] = const_cast<Arc*>(a);
        }
    });
    backwardBfs.onTreeArcDiscover([&forwardBfs,&treeArc,&fbLink](const Arc *a) {
        if (fbLink) {
            return;
        }
        auto tail = a->getTail();
        if (forwardBfs.vertexDiscovered(tail)) {
            fbLink = const_cast<Arc*>(a);
        } else {
            treeArc[tail] = const_cast<Arc*>(a);
        }
    });

    forwardBfs.prepare();
    backwardBfs.prepare();

    forwardBfs.run();
    backwardBfs.run();

    while (!fbLink && !forwardBfs.isExhausted() && !backwardBfs.isExhausted()) {
        forwardBfs.resume();
        backwardBfs.resume();
    }

    arcPath.clear();
    if (fbLink) {
        auto v = fbLink->getTail();
        while (v != from) {
            auto *a = treeArc(v);
            arcPath.push_back(a);
            v = a->getTail();
        }
        std::reverse(arcPath.begin(), arcPath.end());
        arcPath.push_back(fbLink);
        v = fbLink->getHead();
        while (v != to) {
            auto *a = treeArc(v);
            arcPath.push_back(a);
            v = a->getHead();
        }

        assert(!arcPath.empty());
    }
}

template<template <typename T> typename property_map_type>
void FindDiPathAlgorithm<property_map_type>::runOneWaySearch()
{
    BreadthFirstSearch<property_map_type,false> bfs(false, false);
    bfs.setGraph(diGraph);
    property_map_type<Arc*> pred(nullptr);

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

    if (pathFound && (constructVertexPath || constructArcPath)) {
        Vertex *p = to;
        while (p != from) {
            Arc *a = pred(p);
            vertexPath.push_back(p);
            arcPath.push_back(a);
            p = a->getTail();
        }
        std::reverse(arcPath.begin(), arcPath.end());

        if (constructVertexPath) {
            vertexPath.push_back(from);
            for (const auto &a: arcPath) {
                vertexPath.push_back(a->getHead());
            }
        }
    }
}

}
