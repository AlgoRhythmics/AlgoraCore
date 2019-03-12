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

#ifndef FINDDIPATHALGORITHM_H
#define FINDDIPATHALGORITHM_H

#include "algorithm/valuecomputingalgorithm.h"

#include <vector>

namespace Algora {

class Vertex;
class Arc;

class FindDiPathAlgorithm : public ValueComputingAlgorithm<bool>
{
public:
    typedef std::vector<Vertex*>::const_iterator VertexIterator;
    typedef std::vector<Arc*>::const_iterator ArcIterator;

    explicit FindDiPathAlgorithm(bool constructPath = true);
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

    ArcIterator begin() const {
        return arcsOnPathBegin();
    }

    ArcIterator end() const {
        return arcsOnPathEnd();
    }

    VertexIterator verticesOnPathBegin() const {
        return vertexPath.cbegin();
    }

    VertexIterator verticesOnPathEnd() const {
        return vertexPath.cend();
    }

    ArcIterator arcsOnPathBegin() const {
        return arcPath.cbegin();
    }

    ArcIterator arcsOnPathEnd() const {
        return arcPath.cend();
    }

    // DiGraphAlgorithm interface
public:
    virtual bool prepare() override;
    virtual void run() override;
    virtual std::string getName() const noexcept override { return "FindDiPath"; }
    virtual std::string getShortName() const noexcept override { return "dipath"; }

    // ValueComputingAlgorithm interface
public:
    virtual bool deliver() override;

private:
    bool constructPath;
    Vertex *from;
    Vertex *to;
    std::vector<Vertex*> vertexPath;
    std::vector<Arc*> arcPath;
    bool pathFound;

    // DiGraphAlgorithm interface
private:
    virtual void onDiGraphSet() override;
};

}

#endif // FINDDIPATHALGORITHM_H
