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

#ifndef TARJANSCCALGORITHM_H
#define TARJANSCCALGORITHM_H

#include "algorithm/propertycomputingalgorithm.h"
#include "property/propertymap.h"
#include "graph/digraph.h"

#include <limits>

namespace Algora {

class Vertex;

template <template<typename T> class ModifiablePropertyType = PropertyMap>
class TarjanSCCAlgorithm : public PropertyComputingAlgorithm<DiGraph::size_type, DiGraph::size_type>
{
public:
    TarjanSCCAlgorithm();
    virtual ~TarjanSCCAlgorithm() override;

    // DiGraphAlgorithm interface
public:
    virtual void run() override;
    virtual std::string getName() const noexcept override { return "TarjanSCC"; }
    virtual std::string getShortName() const noexcept override { return "tarjan"; }

    // ValueComputingAlgorithm interface
public:
    virtual DiGraph::size_type deliver() override;

private:
    DiGraph::size_type numSccs;
};

}

#include "tarjansccalgorithm.cpp"

#endif // TARJANSCCALGORITHM_H
