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
