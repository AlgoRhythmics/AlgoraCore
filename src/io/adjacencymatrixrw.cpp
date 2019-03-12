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

#include "adjacencymatrixrw.h"

#include "graph/digraph.h"
#include "property/propertymap.h"
#include "pipe/digraphinfo.h"

#include <map>
#include <tuple>
#include <algorithm>
#include <iostream>

#ifdef DEBUG_AMATRIXRW
#define PRINT_DEBUG(msg) std::cout << msg << std::endl;
#define IF_DEBUG(cmd) cmd;
#else
#define PRINT_DEBUG(msg)
#define IF_DEBUG(cmd)
#endif

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
bool writeGraph(std::ostream &os, const DiGraph *graph, const DiGraphInfo *info, bool oneLine, bool upperTriangleOnly, bool includeDiagonal);

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

void AdjacencyMatrixRW::processGraph(const DiGraph *graph, const DiGraphInfo *info)
{
    if (StreamDiGraphWriter::outputStream == 0) {
        return;
    }

    std::ostream &outputStream = *(StreamDiGraphWriter::outputStream);

    writeGraph(outputStream, graph, info, grin->oneLine, grin->upperTriangularMatrix, grin->includeDiagonal);
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
        std::cerr << "io: Could not read n." << std::endl;
        return false;
    }
    if (n <= 0) {
        std::cerr << "io: n <= 0." << std::endl;
        return false;
    }
    PRINT_DEBUG( "n " << n )
    is >> std::ws;

    int p = is.peek();
    PRINT_DEBUG( "p \"" << (char) p << "\"" )
    oneLine = !std::isdigit(p);
    if (oneLine) {
        PRINT_DEBUG( "one line" )
        char c;
        is >> c;
        PRINT_DEBUG( "next char " << c )
        if (c != ':') {
            std::cerr << "io: Missing first ':'." << std::endl;
            return false;
        }
        is >> std::ws;
        while ((is.peek() != ':') && (is >> c)) {
            PRINT_DEBUG( "next char " << c )
            switch (c) {
            case 'u' :
                upperTriangle = true;
                break;
            case 'd':
                diagonal = true;
                break;
            default:
                std::cerr << "io: Unsupported option " << c << "." << std::endl;
                return false;
            }
            is >> std::ws;
        }
        PRINT_DEBUG( "upper triangle " << upperTriangle )
        PRINT_DEBUG( "diagonal " << diagonal )
        if ((!is.good()) || (diagonal && !upperTriangle)) {
            std::cerr << "io: Stream error or illegal option combination." << std::endl;
            return false;
        }
        is >> c;
        PRINT_DEBUG( "next char " << c )
        if (c != ':') {
            std::cerr << "io: Missing second ':'." << std::endl;
            return false;
        }
    }
    std::vector<Vertex*> vertices;
    for (int i = 0; i < n; i++) {
        vertices.push_back(graph->addVertex());
    }

    int weight;
    int numValues = 0;
    for (int i = 0; i < n; i++) {
        int jStart = upperTriangle ? (diagonal ? i : i + 1) : 0;
        for (int j = jStart; j < n; j++) {
            numValues++;
            if (!(is >> weight)) {
                std::cerr << "io: Could not read entry #" << numValues << "=(" << i << ", " << j << ")." << std::endl;
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
    is.ignore(2, '\n');

    return true;
}

bool writeGraph(std::ostream &os, const DiGraph *graph, const DiGraphInfo *info, bool oneLine, bool upperTriangleOnly, bool includeDiagonal) {

    DiGraph *ncGraph = const_cast<DiGraph*>(graph);
    int n = ncGraph->getSize();
    DiGraphInfo defaultInfo(ncGraph);
    if (!info) {
        info = &defaultInfo;
    }

    PropertyMap<int> vertexId(-1);
    int i = 0;
    info->mapVertices([&](Vertex *v) { vertexId[v] = i++; });
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
    info->mapArcsUntil([&](Arc *arc) { createTuple(arc); }, [&](const Arc *) { return !ok; });
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
