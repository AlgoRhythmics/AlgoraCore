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

#ifndef BICONNECTEDCOMPONENTSALGORITHM_H
#define BICONNECTEDCOMPONENTSALGORITHM_H

#include "algorithm/propertycomputingalgorithm.h"
#include <vector>

namespace Algora {

class BiconnectedComponentsAlgorithm : public PropertyComputingAlgorithm<int, std::vector<int> >
{
public:
    BiconnectedComponentsAlgorithm();
    virtual ~BiconnectedComponentsAlgorithm();

    // DiGraphAlgorithm interface
public:
    virtual void run() override;
    virtual std::string getName() const noexcept override { return "BiconnectedComponents"; }
    virtual std::string getShortName() const noexcept override { return "bic"; }

    // ValueComputingAlgorithm interface
public:
    virtual int deliver() override;

private:
    int numBics;
};

}

#endif // BICONNECTEDCOMPONENTSALGORITHM_H
