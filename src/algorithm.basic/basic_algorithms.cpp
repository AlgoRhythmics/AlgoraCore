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

#include "basic_algorithms.h"

#include "graph/digraph.h"
#include "property/propertymap.h"

#include "algorithm.basic.traversal/breadthfirstsearch.h"
#include "algorithm.basic.traversal/depthfirstsearch.h"
#include "tarjansccalgorithm.h"
#include "topsortalgorithm.h"
#include "biconnectedcomponentsalgorithm.h"
#include "eccentricityalgorithm.h"
#include "radiusdiameteralgorithm.h"

namespace Algora {

bool hasDiPath(DiGraph *diGraph, Vertex *from, Vertex *to) {
    FindDiPathAlgorithm findDiPath(false);
    return runDiPathAlgorithm(diGraph, from, to, findDiPath);
}

bool isAcyclic(DiGraph *diGraph) {
    TopSortAlgorithm topSort(false);
    int sortedVertices = runAlgorithm(topSort, diGraph);
    return sortedVertices == (int) diGraph->getSize();
}

bool isStronglyConnected(DiGraph *diGraph)
{
    return countStrongComponents(diGraph) == 1;
}

int countStrongComponents(DiGraph *diGraph)
{
    TarjanSCCAlgorithm tarjan;
    PropertyMap<int> sccs(-1);
    tarjan.useModifiableProperty(&sccs);
    return runAlgorithm(tarjan, diGraph);
}

bool isBiconnected(DiGraph *diGraph)
{
    return countBiconnectedComponents(diGraph) == 1;
}

int countBiconnectedComponents(DiGraph *diGraph)
{
    BiconnectedComponentsAlgorithm bic;
    std::vector<int> empty;
    PropertyMap<std::vector<int> > bics(empty);
    bic.useModifiableProperty(&bics);
    return runAlgorithm(bic, diGraph);
}

bool runDiPathAlgorithm(DiGraph *diGraph, Vertex *from, Vertex *to, FindDiPathAlgorithm &a)
{
    a.setSourceVertex(from);
    a.setTargetVertex(to);
    return runAlgorithm(a, diGraph);
}

int computeEccentricity(DiGraph *diGraph, const Vertex *v)
{
    EccentricityAlgorithm ecc;
    ecc.setVertex(v);
    return runAlgorithm(ecc, diGraph);
}

int computeRadius(DiGraph *diGraph)
{
    RadiusDiameterAlgorithm rd;
    rd.computeRadius(true);
    rd.computeRadiusOnly(true);
    return runAlgorithm(rd, diGraph);
}

int computeDiameter(DiGraph *diGraph)
{
    RadiusDiameterAlgorithm rd;
    rd.computeRadius(false);
    rd.computeDiameterOnly(true);
    return runAlgorithm(rd, diGraph);
}

void computeCondensation(DiGraph *diGraph, DiGraph *condensedGraph)
{
    TarjanSCCAlgorithm tarjan;
    PropertyMap<int> sccOf(-1);
    tarjan.useModifiableProperty(&sccOf);
    int sccs = runAlgorithm(tarjan, diGraph);
    condensedGraph->clear();
    std::vector<Vertex*> sccVertices;
    for (int i = 0; i < sccs; i++) {
        sccVertices.push_back(condensedGraph->addVertex());
    }
    diGraph->mapArcs([&](Arc *a) {
        int tailScc = sccOf(a->getTail());
        int headScc = sccOf(a->getHead());
        if (tailScc != headScc) {
            condensedGraph->addArc(sccVertices.at(tailScc), sccVertices.at(headScc));
        }
    });
}

}


