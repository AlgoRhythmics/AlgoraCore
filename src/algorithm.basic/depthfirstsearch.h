// license
#ifndef DEPTHFIRSTSEARCH_H
#define DEPTHFIRSTSEARCH_H

#include "algorithm/propertycomputingalgorithm.h"

namespace Algora {

class Vertex;

struct DFSResult {
    int dfsNumber = -1;
    int lowNumber = -1;
    Vertex *parent = 0;
    DFSResult(int dfs=-1, int low=-1, Vertex *p=0) :
        dfsNumber(dfs), lowNumber(low), parent(p) {}
    DFSResult(Vertex *p) : parent(p) {}
};

class DepthFirstSearch : public PropertyComputingAlgorithm<int, DFSResult>
{
public:
    DepthFirstSearch();
    virtual ~DepthFirstSearch();

    void setStartVertex(Vertex *v) {
        startVertex = v;
    }

    void setIgnoreArcDirections(bool ignore) {
       ignoreArcDirections = ignore;
    }

    // DiGraphAlgorithm interface
public:
    virtual bool prepare() override;
    virtual void run() override;
    virtual std::string getName() const noexcept override { return "DFS"; }

    // ValueComputingAlgorithm interface
public:
    virtual int deliver() override;

private:
    Vertex *startVertex;
    int maxDfsNumber;
    bool ignoreArcDirections;
};

}

#endif // DEPTHFIRSTSEARCH_H
