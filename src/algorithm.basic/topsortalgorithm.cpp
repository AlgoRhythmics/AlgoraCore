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

#include "topsortalgorithm.h"

#include "graph/digraph.h"
#include "graph/vertex.h"
#include "graph/arc.h"
#include "property/propertymap.h"

#include <vector>

//#define DEBUG_TOPSORT

#ifdef DEBUG_TOPSORT
#include <iostream>
#define PRINT_DEBUG(msg) std::cout << msg << std::endl;
#define IF_DEBUG(cmd) cmd;
#else
#define PRINT_DEBUG(msg)
#define IF_DEBUG(cmd)
#endif

namespace Algora {

TopSortAlgorithm::TopSortAlgorithm(bool computeValues)
    : PropertyComputingAlgorithm(computeValues)
{

}

TopSortAlgorithm::~TopSortAlgorithm()
{

}

bool TopSortAlgorithm::isSortedTopologically(const std::vector<Vertex *> &seq) const
{
    PropertyMap<int> inDegree(-1);

    diGraph->mapVertices([&](Vertex *v) {
        inDegree[v] = diGraph->getInDegree(v, true);
    });

    for (Vertex *v : seq) {
        if (inDegree(v) != 0) {
            return false;
        }
        diGraph->mapOutgoingArcs(v, [&](Arc *a) {
            Vertex *h = a->getHead();
            inDegree[h] -= a->getSize();
        });
    }
    return true;
}

void TopSortAlgorithm::run()
{
    sequence.clear();

    PropertyMap<int> inDegree(-1);

    std::vector<Vertex*> sources;

    diGraph->mapVertices([&](Vertex *v) {
        int indegree = diGraph->getInDegree(v, true);
        if (indegree == 0) {
            sources.push_back(v);
        } else {
            inDegree[v] = indegree;
        }
    });
    PRINT_DEBUG( "Queue contains " << sources.size() << " sources." );

    int ts = 0;
    while (!sources.empty()) {
        Vertex *v = sources.back();
        sources.pop_back();

        if (computePropertyValues) {
            property->setValue(v, ts);
            ts++;
        }

        sequence.push_back(v);

        diGraph->mapOutgoingArcs(v, [&](Arc *a) {
            Vertex *head = a->getHead();
            if (inDegree(head) == 1) {
                sources.push_back(head);
            } else {
                inDegree[head]--;
            }
        });
    }
}

}
