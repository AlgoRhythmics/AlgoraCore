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

#ifndef DIGRAPHALGORITHM_H
#define DIGRAPHALGORITHM_H

#include <string>

namespace Algora {

class DiGraph;

class DiGraphAlgorithm
{
public:
    explicit DiGraphAlgorithm() : diGraph(nullptr) { }
    virtual ~DiGraphAlgorithm() { }

    void setGraph(DiGraph *diGraph) {
        if (this->diGraph != diGraph) {
          onDiGraphUnset();
          this->diGraph = diGraph;
          onDiGraphSet();
        }
    }
    void unsetGraph() { onDiGraphUnset(); this->diGraph = nullptr; }
    bool hasGraph() const { return diGraph != nullptr; }

    virtual bool prepare() { return hasGraph(); }
    virtual void run() = 0;

    virtual std::string getName() const noexcept = 0;
    virtual std::string getShortName() const noexcept = 0;
    virtual std::string getProfilingInfo() const { return ""; }

protected:
    DiGraph *diGraph;
    virtual void onDiGraphSet() { }
    virtual void onDiGraphUnset() { }
};

}

#endif // DIGRAPHALGORITHM_H
