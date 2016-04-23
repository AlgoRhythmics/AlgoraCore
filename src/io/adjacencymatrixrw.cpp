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

bool readGraph(std::istream &is, DiGraph *graph);
bool writeGraph(std::ostream &os, const DiGraph *graph, bool oneLine, bool upperTriangleOnly, bool includeDiagonal);

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

    writeGraph(outputStream, graph, grin->oneLine, grin->upperTriangularMatrix, grin->includeDiagonal);
}

bool AdjacencyMatrixRW::provideDiGraph(DiGraph *graph)
{
    if (StreamDiGraphReader::inputStream == 0) {
        return false;
    }
    std::istream &inputStream = *(StreamDiGraphReader::inputStream);

    return readGraph(inputStream, graph);
}

bool readGraph(std::istream &is, DiGraph *graph) {

    int n = 0;
    bool oneLine = false;
    bool upperTriangle = false;
    bool diagonal = false;

    if (!(is >> n)) {
        return false;
    }
    if (n <= 0) {
        return false;
    }
    is >> std::skipws;

    int p = is.peek();
    oneLine = std::isdigit(p);
    if (oneLine) {
        char c;
        is >> c;
        if (c != ':') {
            return false;
        }
        while ((is.peek() != ':') && (is >> c)) {
            if (!std::isspace(c)) {
                switch (c) {
                case 'u' :
                    upperTriangle = true;
                    break;
                case 'd':
                    diagonal = true;
                    break;
                default:
                    return false;
                }
            }
        }
        if ((!is.good()) || (diagonal && !upperTriangle)) {
            return false;
        }
        is >> c;
        if (c != ':') {
            return false;
        }
    }
    std::vector<Vertex*> vertices;
    for (int i = 0; i < n; i++) {
        vertices.push_back(graph->addVertex());
    }

    int weight;
    for (int i = 0; i < n; i++) {
        int jStart = upperTriangle ? (diagonal ? i : i + 1) : 0;
        for (int j = jStart; j < n; j++) {
            if (!(is >> weight)) {
                return false;
            }
            if (weight == 0) {
                continue;
            }
            Vertex *from;
            Vertex *to;
            if (upperTriangle && weight < 0) {
                weight = -weight;
                from = vertices.at(j);
                to = vertices.at(i);
            } else {
                from = vertices.at(i);
                to = vertices.at(j);
            }
            if (weight == 1) {
                graph->addArc(from, to);
            } else {
               graph->addMultiArc(from, to, weight);
            }
        }
    }

    return true;
}

bool writeGraph(std::ostream &os, const DiGraph *graph, bool oneLine, bool upperTriangleOnly, bool includeDiagonal) {

    DiGraph *ncGraph = const_cast<DiGraph*>(graph);
    int n = ncGraph->getSize();
    PropertyMap<int> vertexId(-1);
    int i = 0;
    ncGraph->mapVertices([&](Vertex *v) { vertexId[v] = i++; });
    std::map<std::pair<int,int>, int> arcsToWeight;

    bool ok = true;
    ArcMapping createTuple = [&](Arc *a) {
        int h = vertexId[a->getHead()];
        int t = vertexId[a->getTail()];
        int size = a->getSize();
        if (oneLine && upperTriangleOnly && !includeDiagonal && h == t) {
            ok = false;
            return;
        }
        if (upperTriangleOnly && h < t) {
            arcsToWeight[std::make_pair(h, t)] = -size;
        } else {
            arcsToWeight[std::make_pair(t, h)] = size;
        }
    };
    ncGraph->mapArcsUntil([&](Arc *arc) { createTuple(arc); }, [&](const Arc *) { return !ok; });
    if (!ok) {
        return false;
    }

    if (oneLine) {
        os << n << " : ";
        if (upperTriangleOnly) {
            os << "u ";
        }
        if (includeDiagonal) {
            os << "d ";
        }
        os << ":";
    } else {
        os << n << std::endl;
    }

    int row = 0;
    int col = oneLine && upperTriangleOnly && !includeDiagonal ? 1 : 0;
    for (auto it = arcsToWeight.cbegin(); it != arcsToWeight.cend(); ++it) {
        auto p = (*it).first;
        int weight = (*it).second;

        while (row < p.first) {
            while (col < n) {
                if (oneLine || col != 0) {
                    os << " ";
                }
                os << "0";
                col++;
            }
            if (!oneLine) {
                os << std::endl;
            }
            row++;
            col = (!oneLine || !upperTriangleOnly) ? 0 : (includeDiagonal ? row : row + 1);
        }
        while (col < p.second) {
            if (oneLine || col != 0) {
                os << " ";
            }
            os << "0";
            col++;
        }
        if (oneLine || col != 0) {
            os << " ";
        }
        os << weight;
        col++;
    }
    while (row < n) {
        while (col < n) {
            if (oneLine || col != 0) {
                os << " ";
            }
            os << "0";
            col++;
        }
        if (!oneLine) {
            os << std::endl;
        }
        row++;
        col = (!oneLine || !upperTriangleOnly) ? 0 : (includeDiagonal ? row : row + 1);
    }
    return true;
}

}
