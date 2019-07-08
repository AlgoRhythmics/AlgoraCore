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

#include "graphartifact.h"

#include <sstream>

namespace Algora {

GraphArtifact::id_type GraphArtifact::nextId = 0ULL;

GraphArtifact::GraphArtifact(id_type id, GraphArtifact *parent)
    : id(id), parent(parent), valid(true)
{

}

GraphArtifact::GraphArtifact(GraphArtifact *parent)
    : id(nextId), parent(parent), valid(true)
{
    nextId++;
}

GraphArtifact::~GraphArtifact()
{
}

GraphArtifact::GraphArtifact(const GraphArtifact &other)
    : id(nextId), parent(other.parent), valid(other.valid), name(other.name)
{
    nextId++;
}

GraphArtifact &GraphArtifact::operator=(const GraphArtifact &other)
{
    if (&other == this) {
        return *this;
    }

    // keep my id
    parent = other.parent;
    valid = other.valid;
    name = other.name;

    return *this;
}

std::string GraphArtifact::idString() const
{
    std::ostringstream strStream;
    strStream << this->typeString();
    if (parent) {
        strStream << std::to_string(parent->getId());
    }
    strStream << "#"
              << std::to_string(getId());
    return strStream.str();
}

}
