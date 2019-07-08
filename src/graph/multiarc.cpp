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

#include "multiarc.h"

#include "vertex.h"

#include <sstream>

namespace Algora {

MultiArc::~MultiArc()
{
}

std::string MultiArc::toString() const
{
    std::ostringstream strStream;
    strStream << typeString()
              << " ("
              << getTail()->toString()
              << " -> "
              << getHead()->toString()
              << ") ["
              << idString()
              << ", size "
              << getSize()
              << "]";

    return strStream.str();
}

MultiArc::MultiArc(Vertex *tail, Vertex *head, GraphArtifact *parent)
    : Arc(tail, head, parent)
{  }

MultiArc::MultiArc(Vertex *tail, Vertex *head, id_type id, GraphArtifact *parent)
    : Arc(tail, head, id, parent)
{  }

}
