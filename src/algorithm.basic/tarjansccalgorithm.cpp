/**
 * Copyright (C) 2013 - 2015 : Kathrin Hanauer
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


#include "tarjansccalgorithm.h"

#include "graph/digraph.h"
#include "graph/vertex.h"
#include "graph/arc.h"
#include "property/propertymap.h"

#include <deque>

namespace Algora {

int tarjanRecursive(DiGraph *diGraph, PropertyMap<int> &sccNumber);
void strongconnect(DiGraph *graph, Vertex *v,
                   int &nextIndex, int &nextScc,
                   std::deque<Vertex*> &stack,
                   PropertyMap<int> &vertexIndex,
                   PropertyMap<int> &lowLink,
                   PropertyMap<bool> &onStack, PropertyMap<int> &sccNumber);

TarjanSCCAlgorithm::TarjanSCCAlgorithm()
    : numSccs(0)
{

}

TarjanSCCAlgorithm::~TarjanSCCAlgorithm()
{

}

void TarjanSCCAlgorithm::run()
{
    numSccs = tarjanRecursive(diGraph, *this->propertyMap);

    if (numSccs > 1) {
        diGraph->visitVertices([&](Vertex *v) {
            propertyMap->setValue(v, numSccs - propertyMap->getValue(v) - 1);
        });
    }
}

int TarjanSCCAlgorithm::deliver()
{
    return numSccs;
}

int tarjanRecursive(DiGraph *diGraph, PropertyMap<int> &sccNumber) {
    int nextIndex = 0;
    int nextScc = 0;
    std::deque<Vertex*> stack;
    PropertyMap<int> vertexIndex(-1);
    PropertyMap<int> lowLink(-1);
    PropertyMap<bool> onStack(false);

    diGraph->visitVertices([&](Vertex *v) {
        if (vertexIndex.getValue(v) == -1) {
            strongconnect(diGraph, v, nextIndex, nextScc, stack, vertexIndex, lowLink, onStack, sccNumber);
        }
    });
    return nextScc;
}

void strongconnect(DiGraph *graph, Vertex *v,
                   int &nextIndex, int &nextScc,
                   std::deque<Vertex *> &stack,
                   PropertyMap<int> &vertexIndex,
                   PropertyMap<int> &lowLink,
                   PropertyMap<bool> &onStack,
                   PropertyMap<int> &sccNumber) {

    int vLowLink = nextIndex;
    vertexIndex.setValue(v, nextIndex);
    lowLink.setValue(v, nextIndex);
    nextIndex++;
    stack.push_back(v);
    onStack.setValue(v, true);

    graph->visitOutgoingArcs(v, [&](Arc *a) {
        Vertex *head = a->getHead();
        if (vertexIndex.getValue(head) == -1) {
            strongconnect(graph, head, nextIndex, nextScc, stack, vertexIndex, lowLink, onStack, sccNumber);
            int hLowLink = lowLink.getValue(head);
            if (hLowLink < vLowLink) {
                lowLink.setValue(v, hLowLink);
            }
        } else if (onStack.getValue(head)) {
            int hIndex = vertexIndex.getValue(head);
            if (hIndex < vLowLink) {
                lowLink.setValue(v, hIndex);
            }
        }
    });

    if (lowLink.getValue(v) == vertexIndex.getValue(v)) {
        Vertex *w;
        do {
            w  = stack.back();
            stack.pop_back();
            onStack.setValue(w, false);
            sccNumber.setValue(w, nextScc);
        } while (w != v);
        nextScc++;
    }
}

}
