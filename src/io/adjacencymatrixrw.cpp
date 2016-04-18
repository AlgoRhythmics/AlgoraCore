/**
 * Copyright (C) 2013 - 2016 : Kathrin Hanauer
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

#include "adjacencymatrixrw.h"

#include "graph/digraph.h"
#include "property/propertymap.h"

#include <map>
#include <tuple>
#include <algorithm>

namespace Algora {

struct AdjacencyMatrixRW::CheshireCat {
    bool oneLine;
    bool upperTriangularMatrix;
    bool includeDiagonal;

    CheshireCat(bool oneLine, bool upper, bool diag)
        : oneLine(oneLine),
          upperTriangularMatrix(upper),
          includeDiagonal(diag) { }
};

bool writeGraphOneLine(std::ostream &os, const DiGraph *graph, bool upperTriangleOnly, bool includeDiagonal);

AdjacencyMatrixRW::AdjacencyMatrixRW(bool oneLine, bool upperTriangleOnly, bool withDiagonal)
    : grin(new CheshireCat(oneLine, upperTriangleOnly, withDiagonal))
{

}

AdjacencyMatrixRW::~AdjacencyMatrixRW()
{
    delete grin;
}

void AdjacencyMatrixRW::useOneLineFormat(bool oneLine)
{
    grin->oneLine = oneLine;
    if (!oneLine) {
        grin->upperTriangularMatrix = false;
        grin->includeDiagonal = true;
    }
}

void AdjacencyMatrixRW::writeUpperTriangleOnly(bool upper)
{
    grin->upperTriangularMatrix = upper;
}

void AdjacencyMatrixRW::writeWithDiagonal(bool diagonal)
{
    grin->includeDiagonal = diagonal;
}

bool AdjacencyMatrixRW::oneLine() const
{
    return grin->oneLine;
}

bool AdjacencyMatrixRW::upperTriangleOnly() const
{
    return grin->upperTriangularMatrix;
}

bool AdjacencyMatrixRW::withDiagonal() const
{
    return grin->includeDiagonal;
}

void AdjacencyMatrixRW::processGraph(const DiGraph *graph)
{
    if (StreamDiGraphWriter::outputStream == 0) {
        return;
    }

    std::ostream &outputStream = *(StreamDiGraphWriter::outputStream);

    if (grin->oneLine) {
        return writeGraphOneLine(outputStream, graph, grin->upperTriangularMatrix, grin->includeDiagonal);
    }

}

bool AdjacencyMatrixRW::provideDiGraph(DiGraph *graph)
{
    if (StreamDiGraphReader::inputStream == 0) {
        return false;
    }
    std::istream &inputStream = *(StreamDiGraphReader::inputStream);
}

bool writeGraphOneLine(std::ostream &os, const DiGraph *graph, bool oneLine, bool upperTriangleOnly, bool includeDiagonal) {

    DiGraph *ncGraph = const_cast<DiGraph*>(graph);
    int n = ncGraph->getSize();
    os << ": " << n << " :";
    if (upperTriangleOnly) {
        os << "u ";
    }
    if (includeDiagonal) {
        os << "d ";
    }
    os << ":";

    PropertyMap<int> vertexId(-1);
    int i = 0;
    ncGraph->mapVertices([&](Vertex *v) { vertexId[v] = i++; });
    std::map<std::pair<int,int>, int> arcsToWeight;

    ArcMapping createTuple = [&](Arc *a) {
        int h = vertexId[a->getHead()];
        int t = vertexId[a->getTail()];
        int size = a->getSize();
        if (upperTriangleOnly && h < t) {
            arcsToWeight[std::make_pair(h, t)] = -size;
        } else {
            arcsToWeight[std::make_pair(t, h)] = size;
        }
    };
    ncGraph->mapArcs([&](Arc *arc) { createTuple(arc); });

    int row = 0;
    int col = -1;
    for (auto it = arcsToWeight.cbegin(); it != arcsToWeight.cend(); ++it) {
        auto p = (*it).first;
        int weight = (*it).second;

        // fill up current row
        int fillUpEnd = p.first == row ? p.second : n;
        for (int j = col + 1; j < fillUpEnd; j++) {
            if (oneLine || j != 0) {
                out << " ";
            }
            out << "0";
        }
        if (p.second != 0) {
            out << " ";
        }

        out << weight;

        for (int i = row; i <= p.first; i++) {
            int jStart = i == row ?
                        col + 1 : (!upperTriangleOnly ? 0 : (includeDiagonal ? i : i + 1));
            for (int j = jStart; j < p.second; j++) {
                if (oneLine || j != jStart) {
                    out << " ";
                }
                out << "0";
            }
            if (!oneLine) {
                out << std::endl;
            }
    }
}

}
