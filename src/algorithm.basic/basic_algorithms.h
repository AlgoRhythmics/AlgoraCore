/**
 * Copyright (C) 2013 - 2016 : Kathrin Hanauer
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
#include "breadthfirstsearch.h"
#include "tarjansccalgorithm.h"
#include "topsortalgorithm.h"

namespace Algora {

class DiGraph;
class Vertex;

bool hasDiPath(DiGraph *diGraph, Vertex *from, Vertex *to);

bool isAcyclic(DiGraph *diGraph);

bool isStronglyConnected(DiGraph *diGraph);

}

#endif // BASIC_ALGORITHMS_H

