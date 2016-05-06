// license
#include "depthfirstsearch.h"

#include "graph/digraph.h"

#include "property/propertymap.h"

#include <iostream>

namespace Algora {

void dfs(DiGraph *g, Vertex *v, int &depth, PropertyMap<std::pair<int, int> > &pm,
         PropertyMap<Vertex *> &parent, PropertyMap<bool> &discovered, bool ignoreDirection);

DepthFirstSearch::DepthFirstSearch()
    : startVertex(0), maxDfsNumber(-1), ignoreArcDirections(false), parentMap(0)
{

}

DepthFirstSearch::~DepthFirstSearch()
{

}

bool DepthFirstSearch::prepare()
{
    return PropertyComputingAlgorithm<bool, std::pair<int, int> >::prepare()
            && (startVertex == 0 || diGraph->containsVertex(startVertex));
}

void DepthFirstSearch::run()
{
    if (startVertex == 0) {
        startVertex = diGraph->getAnyVertex();
    }

    int nextDepth = 0;
    PropertyMap<std::pair<int, int> > &pm = *propertyMap;
    PropertyMap<bool> discovered(false);
    if (!parentMap) {
        PropertyMap<Vertex*> parent(0);
        dfs(diGraph, startVertex, nextDepth, pm, parent, discovered, ignoreArcDirections);
    } else {
        dfs(diGraph, startVertex, nextDepth, pm, *parentMap, discovered, ignoreArcDirections);
    }
    maxDfsNumber = nextDepth - 1;
}

bool DepthFirstSearch::deliver()
{
    return maxDfsNumber + 1 == diGraph->getSize();
}

void dfs(DiGraph *g, Vertex *v, int &depth, PropertyMap<std::pair<int, int> > &pm,
         PropertyMap<Vertex*> &parent,
         PropertyMap<bool> &discovered, bool ignoreDirection) {

    discovered[v] = true;
    int low = depth;
    pm[v] = std::make_pair(depth, low);
    depth++;
    std::cout << v << " : low = " << low << std::endl;

    auto vm = [&](Vertex *v, Vertex *u) {
        std::cout << "Considering child " << u << std::endl;
        if (!discovered(u)) {
            parent[u] = v;
            dfs(g, u, depth, pm, parent, discovered, ignoreDirection);
            if (pm(u).second < low) {
                std::cout << "Updating low from " << low << " to " << pm(u).second << std::endl;
                low = pm(u).second;
                pm[v].second = low;
                std::cout << "Low is now " << pm(v).second << std::endl;
            }
        } else if (parent(v) != u && pm(u).first < low) {
            std::cout << "Updating low from " << low << " to " << pm(u).first << std::endl;
            low = pm(u).first;
            pm[v].second = low;
            std::cout << "Low is now " << pm(v).second << std::endl;
        }
    };
    g->mapOutgoingArcs(v, [&](Arc *a) { vm(v, a->getHead()); });
    if (ignoreDirection) {
        g->mapIncomingArcs(v, [&](Arc *a) { vm(v, a->getTail()); });
    }
}

}
