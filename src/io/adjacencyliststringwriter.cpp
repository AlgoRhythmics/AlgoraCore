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

#include "adjacencyliststringwriter.h"

#include "graph/digraph.h"
#include "graph/vertex.h"
#include "graph/arc.h"
#include "property/propertymap.h"
#include "pipe/digraphinfo.h"

#include <vector>

namespace Algora {

class AdjacencyListStringWriter::CheshireCat {
public:
    AdjacencyListStringFormat format;

    explicit CheshireCat(AdjacencyListStringFormat &f) : format(f) { }
};


AdjacencyListStringWriter::AdjacencyListStringWriter(std::ostream *output, AdjacencyListStringFormat format)
    : StreamDiGraphWriter(output), grin(new CheshireCat(format))
{

}

AdjacencyListStringWriter::~AdjacencyListStringWriter()
{
    delete grin;
}

void AdjacencyListStringWriter::processGraph(const DiGraph *graph, const DiGraphInfo *info)
{
    if (StreamDiGraphWriter::outputStream == 0) {
        return;
    }
    std::ostream &outputStream = *(StreamDiGraphWriter::outputStream);
    DiGraph *ncGraph = const_cast<DiGraph*>(graph);
    outputStream << ncGraph->getSize() << grin->format.getVertexSeparator();

    DiGraphInfo defaultInfo(ncGraph);
    if (!info) {
        info = &defaultInfo;
    }

    std::vector<Vertex*> vertices;
    info->mapVertices([&](Vertex *v) { vertices.push_back(v); });
    PropertyMap<int> vIndex(-1);
    for (unsigned int i = 0; i < vertices.size(); i++) {
        vIndex.setValue(vertices.at(i), i);
    }

    if (grin->format.useOutgoingArcs()) {
        for (unsigned int v = 0; v < vertices.size(); v++) {
            info->mapOutgoingArcs(vertices.at(v), [&](Arc *a) {
                outputStream << vIndex.getValue(a->getHead()) << grin->format.getArcSeparator();
            });
            outputStream << grin->format.getVertexSeparator();
        }
    } else {
        for (unsigned int v = 0; v < vertices.size(); v++) {
            info->mapIncomingArcs(vertices.at(v), [&](Arc *a) {
                outputStream << vIndex.getValue(a->getTail()) << grin->format.getArcSeparator();
            });
            outputStream << grin->format.getVertexSeparator();
        }
    }
    outputStream.flush();
}

}
