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

#include "digraph.h"
#include "arc.h"

#include <sstream>

namespace Algora {

DiGraph::DiGraph(GraphArtifact *parent)
    : Graph(parent)
{

}

DiGraph::DiGraph(const DiGraph &other)
    : Graph(other)
{
    // do not copy listeners
}

DiGraph &DiGraph::operator=(const DiGraph &other)
{
    Graph::operator=(other);
    // do not copy listeners
    return *this;
}

DiGraph::size_type DiGraph::getNumArcs(bool multiArcsAsSimple) const
{
    DiGraph *me = const_cast<DiGraph*>(this);
    size_type numArcs = 0U;
    me->mapVertices([me, &numArcs,multiArcsAsSimple](Vertex *v) {
        numArcs += me->getOutDegree(v, multiArcsAsSimple); });
    return numArcs;
}

void DiGraph::onArcAdd(void *id, const ArcMapping &avFun)
{
    observableArcGreetings.addObserver(id, avFun);
}

void DiGraph::onArcRemove(void *id, const ArcMapping &avFun)
{
    observableArcFarewells.addObserver(id, avFun);
}

void DiGraph::removeOnArcAdd(void *id)
{
    observableArcGreetings.removeObserver(id);
}

void DiGraph::removeOnArcRemove(void *id)
{
    observableArcFarewells.removeObserver(id);
}

void DiGraph::clear()
{
    Graph::clear();
}

std::string DiGraph::toString() const
{
    std::ostringstream strStream;
    strStream << "DiGraph [";
    strStream << idString() << "]";
    return strStream.str();
}

}
