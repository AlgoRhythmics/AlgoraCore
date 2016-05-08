// license
#include "biconnectedcomponentsalgorithm.h"
#include "depthfirstsearch.h"

#include "graph/digraph.h"
#include "property/propertymap.h"

namespace Algora {

int findBiconnectedComponents(DiGraph *graph, std::vector<Vertex *> &dfsOrderRev,
                              PropertyMap<DFSResult> &dfs, PropertyMap<int> &bics);

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
    diGraph->mapVertices([&](Vertex *v) {
        int dfsNum = dfsResult(v).dfsNumber;
        if (dfsNum == -1) {
            dfs.setStartVertex(v);
            runAlgorithm(dfs, diGraph);
        }
        dfsNum = dfsResult(v).dfsNumber;
        dfsOrder[n - 1 - dfsNum] = v;
    });
}

int BiconnectedComponentsAlgorithm::deliver()
{
    return numBics;
}

int findBiconnectedComponents(DiGraph *graph, std::vector<Vertex*> &dfsOrderRev,
                              PropertyMap<DFSResult> &dfs, PropertyMap<int> &bics) {
    int curBic = 0;

    Vertex *curRoot = 0;
    int childrenOfRoot = 0;
    for (Vertex *v : dfsOrderRev) {
        Vertex *p = dfs(v).parent;
        if (!p) {
        }
            if (curRoot != p) {
                curRoot = p;
                childrenOfRoot = 1;
             } else {
                childrenOfRoot++;
            }
        }
    }
}

}
