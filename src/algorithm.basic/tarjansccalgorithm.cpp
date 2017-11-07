/**
 * Copyright (C) 2013 - 2017 : Kathrin Hanauer
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

//#include <iostream>

namespace Algora {

int tarjanRecursive(DiGraph *diGraph, PropertyMap<int> &sccNumber);
void strongconnect(DiGraph *graph, Vertex *v,
                   int &nextIndex, int &nextScc,
                   std::deque<Vertex*> &stack,
                   PropertyMap<int> &vertexIndex,
                   PropertyMap<int> &lowLink,
                   PropertyMap<bool> &onStack,
                   PropertyMap<int> &sccNumber);

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
        diGraph->mapVertices([&](Vertex *v) {
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

    diGraph->mapVertices([&](Vertex *v) {
        if (vertexIndex(v) == -1) {
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

    //std::cout << "strongconnect on " << v << std::endl;
    int vLowLink = nextIndex;
    vertexIndex[v] = nextIndex;
    lowLink[v] = nextIndex;
    //std::cout << "index is " << nextIndex << std::endl;
    //std::cout << "lolink is " << nextIndex << std::endl;
    nextIndex++;
    stack.push_back(v);
    onStack[v] = true;

    graph->mapOutgoingArcs(v, [&](Arc *a) {
        Vertex *head = a->getHead();
        //std::cout << "considering out-neighbor " << head << std::endl;
        if (vertexIndex(head) == -1) {
            //std::cout << "neighbor has no index yet." << std::endl;
            strongconnect(graph, head, nextIndex, nextScc, stack, vertexIndex, lowLink, onStack, sccNumber);
            int hLowLink = lowLink(head);
            //std::cout << "neighbor has lowlink " << hLowLink << std::endl;
            if (hLowLink < vLowLink) {
                vLowLink = hLowLink;
                //std::cout << "lowlink updated." << std::endl;
            }
        } else if (onStack(head)) {
            //std::cout << "neighbor is already on stack." << std::endl;
            int hIndex = vertexIndex(head);
            //std::cout << "neighbor has index " << hIndex << std::endl;
            if (hIndex < vLowLink) {
                vLowLink = hIndex;
                //std::cout << "lowlink updated." << std::endl;
            }
        }
    });
    lowLink[v] = vLowLink;

    if (lowLink(v) == vertexIndex(v)) {
        //std::cout << "Found SCC #" << nextScc << " with members: ";
        Vertex *w;
        do {
            w  = stack.back();
            //std::cout << w << " ";
            stack.pop_back();
            onStack[w] = false;
            sccNumber[w] = nextScc;
        } while (w != v);
        //std::cout << std::endl;
        nextScc++;
    }
    //std::cout << "done with " << v << std::endl;
}

}
