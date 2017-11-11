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

#include "vertex.h"
#include <sstream>

namespace Algora {

Vertex::Vertex(GraphArtifact *parent)
    : GraphArtifact(parent)
{

}

Vertex::~Vertex()
{

}

std::string Vertex::toString() const
{
    std::ostringstream strStream;
    strStream << this;
    return strStream.str();
}

std::ostream &operator<<(std::ostream &out, const Vertex *v)
{
    if (v == 0) {
        out << "<NULL>";
    } else {
        out << "Vertex [" << v->idString() << "]";
    }
    return out;
}

}
