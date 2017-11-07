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

#include "graphartifactproperty.h"

namespace Algora {

class GraphArtifactProperty::CheshireCat {
public:
    std::string name;
    CheshireCat(const std::string &name) : name(name) {}

};

GraphArtifactProperty::GraphArtifactProperty(const std::string &name)
    : grin(new CheshireCat(name))
{

}

GraphArtifactProperty::GraphArtifactProperty(const GraphArtifactProperty &rhs)
    : grin(new CheshireCat(*rhs.grin))
{

}

GraphArtifactProperty::~GraphArtifactProperty()
{
    delete grin;
}

GraphArtifactProperty &GraphArtifactProperty::operator=(const GraphArtifactProperty &rhs)
{
    if (this == &rhs) {
        return *this;
    }
    //grin->name = rhs.grin->name;
    *grin = *(rhs.grin);
    return *this;
}

std::string GraphArtifactProperty::getName() const
{
    return grin->name;
}

void GraphArtifactProperty::setName(const std::string &name)
{
    grin->name = name;
}

}
