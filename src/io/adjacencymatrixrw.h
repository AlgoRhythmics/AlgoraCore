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

#ifndef ADJACENCYMATRIXRW_H
#define ADJACENCYMATRIXRW_H

#include "streamdigraphreader.h"
#include "streamdigraphwriter.h"

namespace Algora {

class AdjacencyMatrixRW : public StreamDiGraphReader, public StreamDiGraphWriter
{
public:
    AdjacencyMatrixRW(bool oneLine = false, bool upperTriangleOnly = false, bool withDiagonal = false);
    virtual ~AdjacencyMatrixRW();

    void useOneLineFormat(bool oneLine);
    void writeUpperTriangleOnly(bool upper);
    void writeWithDiagonal(bool diagonal);

    bool oneLine() const;
    bool upperTriangleOnly() const;
    bool withDiagonal() const;

    // DiGraphProcessor interface
public:
    virtual void processGraph(const DiGraph *graph, const DiGraphInfo *info = 0) override;

    // DiGraphProvider interface
public:
    virtual bool provideDiGraph(DiGraph *graph) override;

private:
    struct CheshireCat;
    CheshireCat *grin;
};

}

#endif // ADJACENCYMATRIXRW_H
