/**
 * Copyright (C) 2013 - 2015 : Kathrin Hanauer
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


#ifndef FINDDIPATHALGORITHM_H
#define FINDDIPATHALGORITHM_H

#include "algorithm/valuecomputingalgorithm.h"

#include <vector>
#include <algorithm>

namespace Algora {

class Vertex;
class Arc;

class FindDiPathAlgorithm : public ValueComputingAlgorithm<bool>
{
public:
    FindDiPathAlgorithm(bool constructPath = true);
    virtual ~FindDiPathAlgorithm();

    void setConstructPath(bool constructPath) {
        this->constructPath = constructPath;
    }

    void setSourceVertex(Vertex *s) {
        from = s;
    }

    void setTargetVertex(Vertex *t) {
        to = t;
    }

    template<typename Container>
    void getVerticesOnDiPath(Container &c) {
        std::copy (vertexPath.cbegin(), vertexPath.cend(), std::back_inserter(c));
    }
    template<typename Container>
    void getArcsOnDiPath(Container &c) {
        std::copy(arcPath.cbegin(), arcPath.cend(), std::back_inserter(c));
    }

    // DiGraphAlgorithm interface
public:
    virtual bool prepare() override;
    virtual void run() override;

    // ValueComputingAlgorithm interface
public:
    virtual bool deliver() override;

private:
    bool constructPath;
    Vertex *from;
    Vertex *to;
    std::vector<Vertex*> vertexPath;
    std::vector<Arc*> arcPath;

    // DiGraphAlgorithm interface
private:
    virtual void onDiGraphSet() override;
};

}

#endif // FINDDIPATHALGORITHM_H
