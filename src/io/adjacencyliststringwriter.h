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

#ifndef ADJACENCYLISTSTRINGWRITER_H
#define ADJACENCYLISTSTRINGWRITER_H

#include "streamdigraphwriter.h"
#include "adjacencyliststringformat.h"

namespace Algora {

class AdjacencyListStringWriter : public StreamDiGraphWriter
{
public:
    AdjacencyListStringWriter(std::ostream *output = 0,
                              AdjacencyListStringFormat format = AdjacencyListStringFormat());
    virtual ~AdjacencyListStringWriter();

    // DiGraphProcessor interface
public:
    virtual void processGraph(const DiGraph *graph, const DiGraphInfo *info = 0) override;

private:
    class CheshireCat;
    CheshireCat *grin;
};

}

#endif // ADJACENCYLISTSTRINGWRITER_H
