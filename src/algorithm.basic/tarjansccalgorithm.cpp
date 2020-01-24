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

#include "tarjansccalgorithm.h"

#include "graph/digraph.h"
#include "graph/vertex.h"
#include "graph/arc.h"
#include "property/propertymap.h"

#include <vector>

//#define DEBUG_TSCC
#ifdef DEBUG_TSCC
#include <iostream>
#define PRINT_DEBUG(msg) std::cout << msg << std::endl;
#define PRINT_DEBUG_CL(msg) std::cout << msg << std::flush;
#define IF_DEBUG(cmd) cmd;
#else
#define PRINT_DEBUG(msg)
#define PRINT_DEBUG_CL(msg)
#define IF_DEBUG(cmd)
#endif

namespace Algora {

template <template<typename T> class ModifiablePropertyType = PropertyMap>
int tarjanRecursive(DiGraph *diGraph, ModifiableProperty<int> &sccNumber);

void strongconnect(DiGraph *graph, Vertex *v,
                   int &nextIndex, int &nextScc,
                   std::vector<Vertex*> &stack,
                   ModifiableProperty<int> &vertexIndex,
                   ModifiableProperty<int> &lowLink,
                   ModifiableProperty<bool> &onStack,
                   ModifiableProperty<int> &sccNumber);

template <template<typename T> class ModifiablePropertyType>
TarjanSCCAlgorithm<ModifiablePropertyType>::TarjanSCCAlgorithm()
    : numSccs(0)
{

}

template <template<typename T> class ModifiablePropertyType>
TarjanSCCAlgorithm<ModifiablePropertyType>::~TarjanSCCAlgorithm()
{

}

template <template<typename T> class ModifiablePropertyType>
void TarjanSCCAlgorithm<ModifiablePropertyType>::run()
{
    numSccs = tarjanRecursive<ModifiablePropertyType>(diGraph, *this->property);

    if (numSccs > 1) {
        diGraph->mapVertices([&](Vertex *v) {
            property->setValue(v, numSccs - property->getValue(v) - 1);
        });
    }
}

template <template<typename T> class ModifiablePropertyType>
int TarjanSCCAlgorithm<ModifiablePropertyType>::deliver()
{
    return numSccs;
}

template <template<typename T> class ModifiablePropertyType>
int tarjanRecursive(DiGraph *diGraph, ModifiableProperty<int> &sccNumber) {
    int nextIndex = 0;
    int nextScc = 0;
    std::vector<Vertex*> stack;
    ModifiablePropertyType<int> vertexIndex(-1);
    ModifiablePropertyType<int> lowLink(-1);
    ModifiablePropertyType<bool> onStack(false);

    diGraph->mapVertices([&](Vertex *v) {
        if (vertexIndex(v) == -1) {
            strongconnect(diGraph, v, nextIndex, nextScc, stack, vertexIndex, lowLink, onStack, sccNumber);
        }
    });
    return nextScc;
}

void strongconnect(DiGraph *graph, Vertex *v,
                   int &nextIndex, int &nextScc,
                   std::vector<Vertex *> &stack,
                   ModifiableProperty<int> &vertexIndex,
                   ModifiableProperty<int> &lowLink,
                   ModifiableProperty<bool> &onStack,
                   ModifiableProperty<int> &sccNumber) {

    PRINT_DEBUG( "strongconnect on " << v )
    int vLowLink = nextIndex;
    vertexIndex.setValue(v, nextIndex);
    lowLink.setValue(v, nextIndex);
    PRINT_DEBUG( "index is " << nextIndex )
    PRINT_DEBUG( "lolink is " << nextIndex )
    nextIndex++;
    stack.push_back(v);
    onStack.setValue(v, true);

    graph->mapOutgoingArcs(v, [&](Arc *a) {
        Vertex *head = a->getHead();
        PRINT_DEBUG( "considering out-neighbor " << head )
        if (vertexIndex(head) == -1) {
            PRINT_DEBUG( "neighbor has no index yet." )
            strongconnect(graph, head, nextIndex, nextScc, stack, vertexIndex, lowLink, onStack, sccNumber);
            int hLowLink = lowLink(head);
            PRINT_DEBUG( "neighbor has lowlink " << hLowLink )
            if (hLowLink < vLowLink) {
                vLowLink = hLowLink;
                PRINT_DEBUG( "lowlink updated." )
            }
        } else if (onStack(head)) {
            PRINT_DEBUG( "neighbor is already on stack." )
            int hIndex = vertexIndex(head);
            PRINT_DEBUG( "neighbor has index " << hIndex )
            if (hIndex < vLowLink) {
                vLowLink = hIndex;
                PRINT_DEBUG( "lowlink updated." )
            }
        }
    });
    lowLink.setValue(v, vLowLink);

    if (lowLink(v) == vertexIndex(v)) {
        PRINT_DEBUG_CL( "Found SCC #" << nextScc << " with members: " )
        Vertex *w;
        do {
            w  = stack.back();
            PRINT_DEBUG_CL( w << " " );
            stack.pop_back();
            onStack.setValue(w, false);
            sccNumber.setValue(w, nextScc);
        } while (w != v);
        PRINT_DEBUG( "" )
        nextScc++;
    }
    PRINT_DEBUG( "done with " << v )
}

}
