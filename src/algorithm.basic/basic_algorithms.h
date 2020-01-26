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

#ifndef BASIC_ALGORITHMS_H
#define BASIC_ALGORITHMS_H

#include "finddipathalgorithm.h"

namespace Algora {

class DiGraph;
class Vertex;

bool hasDiPath(DiGraph *diGraph, Vertex *from, Vertex *to);

bool runDiPathAlgorithm(DiGraph *diGraph, Vertex *from, Vertex *to, FindDiPathAlgorithm<> &a);

bool isAcyclic(DiGraph *diGraph);

bool isStronglyConnected(DiGraph *diGraph);

DiGraph::size_type countStrongComponents(DiGraph *diGraph);

bool isBiconnected(DiGraph *diGraph);

int countBiconnectedComponents(DiGraph *diGraph);

int computeEccentricity(DiGraph *diGraph, const Vertex *v);

int computeRadius(DiGraph *diGraph);

int computeDiameter(DiGraph *diGraph);

void computeCondensation(DiGraph *diGraph, DiGraph *condensedGraph);

}

#endif // BASIC_ALGORITHMS_H

