// license
#ifndef DEPTHFIRSTSEARCH_H
#define DEPTHFIRSTSEARCH_H

#include "algorithm/propertycomputingalgorithm.h"

namespace Algora {

class Vertex;

class DepthFirstSearch : public PropertyComputingAlgorithm<bool, std::pair<int, int> >
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

    virtual void rememberParents(PropertyMap<Vertex*> *pMap) {
        parentMap = pMap;
    }

    // DiGraphAlgorithm interface
public:
    virtual bool prepare() override;
    virtual void run() override;

    // ValueComputingAlgorithm interface
public:
    virtual bool deliver() override;

private:
    Vertex *startVertex;
    int maxDfsNumber;
    bool ignoreArcDirections;
    PropertyMap<Vertex*> *parentMap;
};

}

#endif // DEPTHFIRSTSEARCH_H
