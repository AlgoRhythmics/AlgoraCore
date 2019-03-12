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

#ifndef TOPSORTALGORITHM_H
#define TOPSORTALGORITHM_H

#include "algorithm/propertycomputingalgorithm.h"

#include <vector>

namespace Algora {

class Vertex;

class TopSortAlgorithm : public PropertyComputingAlgorithm<unsigned int,int>
{
public:
    typedef std::vector<Vertex*>::const_iterator VertexIterator;

    explicit TopSortAlgorithm(bool computeValues = true);
    virtual ~TopSortAlgorithm();

    VertexIterator begin() const {
        return sequence.cbegin();
    }

    VertexIterator end() const {
        return sequence.cend();
    }

    bool isSortedTopologically(const std::vector<Vertex*> &seq) const;

    // DiGraphAlgorithm interface
public:
    virtual void run() override;
    virtual std::string getName() const noexcept override { return "TopSort"; }
    virtual std::string getShortName() const noexcept override { return "topsort"; }

private:
    virtual void onDiGraphSet() override { sequence.clear(); }

    // ValueComputingAlgorithm interface
public:
    virtual unsigned int deliver() override { return sequence.size(); }

private:
    std::vector<Vertex*> sequence;
};

}

#endif // TOPSORTALGORITHM_H
