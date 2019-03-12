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

#ifndef VALUECOMPUTINGALGORITHM_H
#define VALUECOMPUTINGALGORITHM_H

#include "digraphalgorithm.h"
#include <stdexcept>

namespace Algora {

template<typename ValueType>
class ValueComputingAlgorithm : public DiGraphAlgorithm
{
public:
    ValueComputingAlgorithm() { }
    virtual ~ValueComputingAlgorithm() { }

    virtual ValueType deliver() = 0;
};

template<typename ValueType>
ValueType runAlgorithm(ValueComputingAlgorithm<ValueType> &algorithm, DiGraph *diGraph) {
    algorithm.setGraph(diGraph);
    if (!algorithm.prepare()) {
        throw std::invalid_argument("Preparing algorithm on given graph failed.");
    }
    algorithm.run();
    return algorithm.deliver();
}

template<typename AlgorithmType, typename ValueType>
ValueType createAndRunAlgorithm(DiGraph *diGraph) {
    AlgorithmType algorithm;
    return runAlgorithm<ValueType>(algorithm, diGraph);
}

}

#endif // VALUECOMPUTINGALGORITHM_H
