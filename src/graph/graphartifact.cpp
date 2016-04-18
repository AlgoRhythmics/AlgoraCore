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


#include "graphartifact.h"

#include <sstream>

namespace Algora {

class GraphArtifact::CheshireCat {
    static int nextId;
public:
    int id;
    GraphArtifact *parent;

    CheshireCat(GraphArtifact *parent);
};

int GraphArtifact::CheshireCat::nextId = 0;

GraphArtifact::GraphArtifact(GraphArtifact *parent)
    : grin(new CheshireCat(parent))
{
}

GraphArtifact::~GraphArtifact()
{
    delete grin;
}

int GraphArtifact::getId() const
{
    return grin->id;
}

GraphArtifact *GraphArtifact::getParent() const
{
    return grin->parent;
}

std::string GraphArtifact::idString() const
{
    std::ostringstream strStream;
    strStream << this->typeString()
              << "#"
              << std::to_string(getId());
    return strStream.str();
}

GraphArtifact::CheshireCat::CheshireCat(GraphArtifact *parent)
    : id(nextId), parent(parent) {
    nextId++;
}

}
