// license
#include "depthfirstsearch.h"

#include "graph/digraph.h"

#include "property/propertymap.h"

#include <iostream>

namespace Algora {

void dfs(DiGraph *g, Vertex *v, int &depth, PropertyMap<DFSResult> &pm,
         PropertyMap<bool> &discovered, bool ignoreDirection);

DepthFirstSearch::DepthFirstSearch()
    : startVertex(0), maxDfsNumber(-1), ignoreArcDirections(false)
{

}

DepthFirstSearch::~DepthFirstSearch()
{

}

bool DepthFirstSearch::prepare()
{
    return PropertyComputingAlgorithm<bool, DFSResult>::prepare()
            && (startVertex == 0 || diGraph->containsVertex(startVertex));
}

void DepthFirstSearch::run()
{
    if (startVertex == 0) {
        startVertex = diGraph->getAnyVertex();
    }

    int nextDepth = 0;
    PropertyMap<DFSResult> &pm = *propertyMap;
    PropertyMap<bool> discovered(false);
    dfs(diGraph, startVertex, nextDepth, pm, discovered, ignoreArcDirections);
    maxDfsNumber = nextDepth - 1;
}

bool DepthFirstSearch::deliver()
{
    return maxDfsNumber + 1 == diGraph->getSize();
}

void dfs(DiGraph *g, Vertex *v, int &depth, PropertyMap<DFSResult> &pm,
         PropertyMap<bool> &discovered, bool ignoreDirection) {

    discovered[v] = true;
    DFSResult &cur = pm[v];
    cur.dfsNumber = depth;
    cur.lowNumber = depth;
    depth++;
    //std::cout << v << " : low = " << cur.lowNumber << std::endl;

    auto vm = [&](Vertex *v, Vertex *u) {
        //std::cout << "Considering child " << u << std::endl;
        if (!discovered(u)) {
            pm[u] = DFSResult(v);
            //std::cout << "Set parent of " << u << " to " << pm(u).parent << std::endl;
            dfs(g, u, depth, pm, discovered, ignoreDirection);
            if (pm(u).lowNumber < cur.lowNumber) {
                //std::cout << "Updating low from " << cur.lowNumber << " to " << pm(u).lowNumber << std::endl;
                cur.lowNumber = pm(u).lowNumber;
                //std::cout << "Low is now " << pm(v).lowNumber << std::endl;
            }
        } else if (cur.parent != u && pm(u).dfsNumber < cur.lowNumber) {
            //std::cout << "Updating low from " << cur.lowNumber << " to " << pm(u).dfsNumber << std::endl;
            cur.lowNumber = pm(u).dfsNumber;
            //std::cout << "Low is now " << pm(v).lowNumber << std::endl;
        }
    };
    g->mapOutgoingArcs(v, [&](Arc *a) { vm(v, a->getHead()); });
    if (ignoreDirection) {
        g->mapIncomingArcs(v, [&](Arc *a) { vm(v, a->getTail()); });
    }
}

}
