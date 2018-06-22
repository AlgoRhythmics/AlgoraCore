#include "radiusdiameteralgorithm.h"

#include "eccentricityalgorithm.h"
#include "graph/digraph.h"

#include <climits>

namespace Algora {

const int RadiusDiameterAlgorithm::INFINITE = INT_MAX;

RadiusDiameterAlgorithm::RadiusDiameterAlgorithm(bool radiusOnly, bool diameterOnly)
    : ValueComputingAlgorithm<int>(), radius(INFINITE), diameter(INFINITE),
      radOrDiam(true), radOnly(radiusOnly), diamOnly(diameterOnly)
{

}

void RadiusDiameterAlgorithm::run()
{
    radius = INT_MAX;
    diameter = -1;
    EccentricityAlgorithm ecc;
    bool stop = false;
    diGraph->mapVerticesUntil([&](Vertex *v) {
        ecc.setVertex(v);
        int e = runAlgorithm(ecc, diGraph);
        if (e > diameter) {
            diameter = e;
            if (diamOnly && diameter == INFINITE) {
                stop = true;
            }
        }
        if (e < radius) {
            radius = e;
            if (radOnly && radius == 1U) {
                stop = true;
            }
        }
    }, [&stop](const Vertex *) { return stop; });
}

void RadiusDiameterAlgorithm::onDiGraphSet()
{
    radius = INFINITE;
    diameter = INFINITE;
}

}
