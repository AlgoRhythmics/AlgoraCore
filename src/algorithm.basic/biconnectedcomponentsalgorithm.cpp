// license
#include "biconnectedcomponentsalgorithm.h"
#include "depthfirstsearch.h"

#include "graph/digraph.h"
#include "property/propertymap.h"

#include <iostream>

namespace Algora {

int findBiconnectedComponents(std::vector<Vertex *> &dfsOrderRev,
                              PropertyMap<DFSResult> &dfs, PropertyMap<std::vector<int> > &bics);

BiconnectedComponentsAlgorithm::BiconnectedComponentsAlgorithm()
    : numBics(0)
{

}

BiconnectedComponentsAlgorithm::~BiconnectedComponentsAlgorithm()
{
}

void BiconnectedComponentsAlgorithm::run()
{
    DFSResult none(-1, -1, 0);
    PropertyMap<DFSResult> dfsResult(none);
    DepthFirstSearch dfs;
    dfs.usePropertyMap(&dfsResult);
    dfs.setIgnoreArcDirections(true);

    int n = diGraph->getSize();
    std::vector<Vertex*> dfsOrderRev(n, 0);
    int verticesReached = 0;
    diGraph->mapVertices([&](Vertex *v) {
        int dfsNum = dfsResult(v).dfsNumber;
        if (dfsNum == -1) {
            //std::cout << "Running DFS starting from " << v << std::endl;
            dfs.setStartVertex(v);
            verticesReached += runAlgorithm(dfs, diGraph);
        }
        dfsNum = dfsResult(v).dfsNumber;
        dfsOrderRev[verticesReached - 1 - dfsNum] = v;
    });
    numBics = findBiconnectedComponents(dfsOrderRev, dfsResult, *propertyMap);
}

int BiconnectedComponentsAlgorithm::deliver()
{
    return numBics;
}

int findBiconnectedComponents(std::vector<Vertex*> &dfsOrderRev,
                              PropertyMap<DFSResult> &dfs, PropertyMap<std::vector<int> > &bics) {
    // cut(v) <=> (v, u) in DfsTree : lowNumber(u) >= dfsNumber(v), if v not root
    //            v has > 1 child in DfsTree, if v is root
    int curBic = 0;

    for (Vertex *u : dfsOrderRev) {
        Vertex *v = dfs(u).parent;
        if (!v) {
            // u is root
            //std::cout << u << " is root." << std::endl;
            // BUGFIX: If u is isolated, it forms a separate BIC
            if (bics(u).empty()) {
                bics[u].push_back(curBic);
                curBic++;
            }
            continue;
        }
        bics[u].push_back(curBic);
        //std::cout << u << " belongs to BIC " << curBic << "." << std::endl;
        // is v cut vertex or root?
        if (dfs(u).lowNumber >= dfs(v).dfsNumber) {
            // v is a cut vertex or root and u is its child
            //std::cout << v << " belongs to BIC " << curBic << " and is cut vertex or root." << std::endl;
            bics[v].push_back(curBic);
            curBic++;
        }
    }
    return curBic;
}

}
