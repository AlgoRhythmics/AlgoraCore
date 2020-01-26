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
#include <limits>

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

namespace  {
const static DiGraph::size_type UNSET = std::numeric_limits<DiGraph::size_type>::max();
}

template <template<typename T> class ModifiablePropertyType = PropertyMap>
DiGraph::size_type tarjanRecursive(DiGraph *diGraph,
                                   ModifiableProperty<DiGraph::size_type> &sccNumber);

void strongconnect(DiGraph *graph, Vertex *v,
                   DiGraph::size_type &nextIndex, DiGraph::size_type &nextScc,
                   std::vector<Vertex*> &stack,
                   ModifiableProperty<DiGraph::size_type> &vertexIndex,
                   ModifiableProperty<DiGraph::size_type> &lowLink,
                   ModifiableProperty<bool> &onStack,
                   ModifiableProperty<DiGraph::size_type> &sccNumber);

template <template<typename T> class ModifiablePropertyType>
TarjanSCCAlgorithm<ModifiablePropertyType>::TarjanSCCAlgorithm()
    : PropertyComputingAlgorithm<DiGraph::size_type, DiGraph::size_type>(true), numSccs(0)
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
            property->setValue(v,
                               numSccs - property->getValue(v) - 1);
        });
    }
}

template <template<typename T> class ModifiablePropertyType>
DiGraph::size_type TarjanSCCAlgorithm<ModifiablePropertyType>::deliver()
{
    return numSccs;
}

template <template<typename T> class ModifiablePropertyType>
GraphArtifact::size_type tarjanRecursive(DiGraph *diGraph,
                                         ModifiableProperty<DiGraph::size_type> &sccNumber) {
    DiGraph::size_type nextIndex = 0;
    DiGraph::size_type nextScc = 0;
    std::vector<Vertex*> stack;
    ModifiablePropertyType<DiGraph::size_type> vertexIndex(UNSET);
    ModifiablePropertyType<DiGraph::size_type> lowLink(UNSET);
    ModifiablePropertyType<bool> onStack(false);

    diGraph->mapVertices([&](Vertex *v) {
        if (vertexIndex(v) == UNSET) {
            strongconnect(diGraph, v, nextIndex, nextScc, stack, vertexIndex, lowLink, onStack, sccNumber);
        }
    });
    return nextScc;
}

void strongconnect(DiGraph *graph, Vertex *v,
                   GraphArtifact::size_type &nextIndex, GraphArtifact::size_type &nextScc,
                   std::vector<Vertex *> &stack,
                   ModifiableProperty<DiGraph::size_type> &vertexIndex,
                   ModifiableProperty<DiGraph::size_type> &lowLink,
                   ModifiableProperty<bool> &onStack,
                   ModifiableProperty<DiGraph::size_type> &sccNumber) {

    PRINT_DEBUG( "strongconnect on " << v )
    auto vLowLink = nextIndex;
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
        if (vertexIndex(head) == UNSET) {
            PRINT_DEBUG( "neighbor has no index yet." )
            strongconnect(graph, head, nextIndex, nextScc, stack, vertexIndex, lowLink, onStack, sccNumber);
            auto hLowLink = lowLink(head);
            PRINT_DEBUG( "neighbor has lowlink " << hLowLink )
            if (hLowLink < vLowLink) {
                vLowLink = hLowLink;
                PRINT_DEBUG( "lowlink updated." )
            }
        } else if (onStack(head)) {
            PRINT_DEBUG( "neighbor is already on stack." )
            auto hIndex = vertexIndex(head);
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
