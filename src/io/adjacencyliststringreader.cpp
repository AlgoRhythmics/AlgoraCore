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

#include "adjacencyliststringreader.h"

#include "graph/digraph.h"

#include <vector>
#include <stdexcept>
#include <sstream>

namespace Algora {

class AdjacencyListStringReader::CheshireCat {
public:
    AdjacencyListStringFormat format;
    std::string lastError;

    explicit CheshireCat(AdjacencyListStringFormat &f) : format(f) { }
};

bool parseInt(std::string s, int *i, std::string &err);

AdjacencyListStringReader::AdjacencyListStringReader(std::istream *input, AdjacencyListStringFormat format)
    : StreamDiGraphReader(input), grin(new CheshireCat(format))
{

}

AdjacencyListStringReader::~AdjacencyListStringReader()
{
    delete grin;
}

std::string AdjacencyListStringReader::getLastError() const
{
    return grin->lastError;
}

bool AdjacencyListStringReader::provideDiGraph(DiGraph *graph)
{
    if (StreamDiGraphReader::inputStream == nullptr) {
        return false;
    }
    std::istream &inputStream = *(StreamDiGraphReader::inputStream);
    using namespace std;
    string token;

    getline(inputStream, token, grin->format.getVertexSeparator());

    if (!inputStream) {
        grin->lastError = "Failed to read number of vertices";
        return false;
    }

    int numVertices;
    if (!parseInt(token, &numVertices, grin->lastError)) {
        return false;
    }

    vector<Vertex*> vertices;
    for (int i = 0; i < numVertices; i++) {
        vertices.push_back(graph->addVertex());
    }

    int currVertex = 0;
    int adjVertex;
    while (currVertex < numVertices
           && getline(inputStream, token, grin->format.getVertexSeparator())) {

        istringstream adjacencyStream(token);

        while (getline(adjacencyStream, token, grin->format.getArcSeparator())) {
            if (!parseInt(token, &adjVertex, grin->lastError)) {
                return false;
            }
            if (adjVertex < 0 || adjVertex >= numVertices) {
                ostringstream stringStream;
                stringStream << "Illegal adjacency " << adjVertex << ".";
                grin->lastError = stringStream.str();
                return false;
            }
            if (grin->format.useOutgoingArcs()) {
                graph->addArc(vertices.at(currVertex), vertices.at(adjVertex));
            } else {
                graph->addArc(vertices.at(adjVertex), vertices.at(currVertex));
            }
        }
        currVertex++;
    }

    return true;

}

bool parseInt(std::string s, int *i, std::string &err) {
    using namespace std;
    size_t pos;
    try {
        *i = stoi(s, &pos);
    } catch (const logic_error &) {
        ostringstream stringStream;
        stringStream << s << " is not an integer.";
        err = stringStream.str();
        return false;
    }
    if (pos < s.length()) {
        ostringstream stringStream;
        stringStream << "Illegal character \"" << s.at(pos) << "\" found.";
        err = stringStream.str();
        return false;
    }

    return true;
}

}
