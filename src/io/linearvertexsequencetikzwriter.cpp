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

#include "linearvertexsequencetikzwriter.h"
#include "graph/digraph.h"
#include "pipe/digraphinfo.h"
#include "property/propertymap.h"

namespace Algora {

class LinearVertexSequenceTikzWriter::CheshireCat {
public:
    int vGap;
    int writtenSequences;

    CheshireCat() : vGap(5), writtenSequences(0) { }
};

LinearVertexSequenceTikzWriter::LinearVertexSequenceTikzWriter(std::ostream *output)
    : StreamDiGraphWriter(output), grin(new CheshireCat)
{

}

LinearVertexSequenceTikzWriter::~LinearVertexSequenceTikzWriter()
{
    delete grin;
}

void LinearVertexSequenceTikzWriter::processGraph(const DiGraph *graph, const DiGraphInfo *info)
{
    if (StreamDiGraphWriter::outputStream == 0 || graph->isEmpty()) {
        return;
    }
    std::ostream &outputStream = *(StreamDiGraphWriter::outputStream);
    DiGraph *ncGraph = const_cast<DiGraph*>(graph);

    DiGraphInfo defaultInfo(ncGraph);
    if (!info) {
        info = &defaultInfo;
    }

    std::vector<Vertex*> vertices;
    info->mapVertices([&](Vertex *v) { vertices.push_back(v); });
    PropertyMap<int> vIndex(-1);
    PropertyMap<std::string> vId("NO_ID");
    for (unsigned int i = 0; i < vertices.size(); i++) {
        vIndex[vertices.at(i)] = i;
        vId[vertices.at(i)] = "v" + std::to_string(i);
    }

    int yCoord = grin->vGap * grin->writtenSequences;
    Vertex *cur = vertices.front();

    outputStream << "\\node[vertex] ("
                 << vId[cur]
                 << ") at (0,"
                 << yCoord
                 << ") { "
                 << info->getVertexName(cur)
                 << " };"
                 << std::endl;

    Vertex *last = cur;
    for (unsigned int i = 1; i < vertices.size(); i++) {
        cur = vertices.at(i);
        outputStream << "\\node[vertex] ("
                     << vId[cur]
                     << ") [right=of "
                     << vId[last]
                     << "] { "
                     << info->getVertexName(cur)
                     << " };"
                     << std::endl;
        last = cur;
    }

    for (unsigned int i = 0; i < vertices.size(); i++) {
        cur = vertices.at(i);
        outputStream << "% outgoing arcs of " << info->getVertexName(cur) << std::endl;
        info->mapOutgoingArcs(cur, [&](Arc *a) {
            outputStream << "\\draw[arc] ("
                         << vId[cur];
            if (vIndex(a->getHead()) - i == 1) {
                outputStream << ") to (";
            } else {
                outputStream << ") to[bend left] (";
            }
            outputStream << vId[a->getHead()]
                         << ");"
                         << std::endl;

        });
    }
    outputStream.flush();
}

}
