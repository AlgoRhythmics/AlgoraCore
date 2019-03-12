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

#ifndef ECCENTRICITYALGORITHM_H
#define ECCENTRICITYALGORITHM_H

#include "algorithm/valuecomputingalgorithm.h"

namespace Algora {

class Vertex;

class EccentricityAlgorithm : public ValueComputingAlgorithm<int>
{
public:
    static const int INFINITE;

    explicit EccentricityAlgorithm();
    virtual ~EccentricityAlgorithm() {}

    void setVertex(const Vertex *v) { vertex = v; eccentricity = INFINITE; }

    // DiGraphAlgorithm interface
public:
    virtual bool prepare() override;
    virtual void run() override;
    virtual std::string getName() const noexcept override { return "Eccentricity Algorithm"; }
    virtual std::string getShortName() const noexcept override { return "Eccentricity"; }

protected:
    virtual void onDiGraphSet() override;

    // ValueComputingAlgorithm interface
public:
    virtual int deliver() override { return eccentricity; }

private:
    const Vertex *vertex;
    int eccentricity;
};

}


#endif // ECCENTRICITYALGORITHM_H
