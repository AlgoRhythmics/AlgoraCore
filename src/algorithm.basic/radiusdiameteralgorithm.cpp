#include "radiusdiameteralgorithm.h"

#include "eccentricityalgorithm.h"
#include "graph/digraph.h"

#include <climits>

namespace Algora {

const int RadiusDiameterAlgorithm::INFINITE = INT_MAX;

RadiusDiameterAlgorithm::RadiusDiameterAlgorithm()
    : ValueComputingAlgorithm<int>(), radius(INFINITE), diameter(INFINITE)
{
}

void RadiusDiameterAlgorithm::run()
{
    radius = INT_MAX;
    diameter = -1;
    EccentricityAlgorithm ecc;
    diGraph->mapVertices([&](Vertex *v) {
        ecc.setVertex(v);
        int e = runAlgorithm(ecc, diGraph);
        if (e > diameter) {
            diameter = e;
        }
        if (e < radius) {
            radius = e;
        }
    });
}

void RadiusDiameterAlgorithm::onDiGraphSet()
{
    radius = INFINITE;
    diameter = INFINITE;
}

}
