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

#ifndef ADJACENCYLISTSTRINGREADER_H
#define ADJACENCYLISTSTRINGREADER_H

#include "streamdigraphreader.h"
#include "adjacencyliststringformat.h"

namespace Algora {

class AdjacencyListStringReader : public StreamDiGraphReader
{
public:
    AdjacencyListStringReader(std::istream *input,
                              AdjacencyListStringFormat format = AdjacencyListStringFormat());
    virtual ~AdjacencyListStringReader() override;

    std::string getLastError() const;

    // DiGraphProvider interface
public:
    virtual bool provideDiGraph(DiGraph *graph) override;

private:
    class CheshireCat;
    CheshireCat *grin;
};

}

#endif // ADJACENCYLISTSTRINGREADER_H
