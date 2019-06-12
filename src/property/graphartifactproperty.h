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

#ifndef GRAPHARTIFACTPROPERTY_H
#define GRAPHARTIFACTPROPERTY_H

#include <string>

namespace Algora {

//class GraphArtifact;

class GraphArtifactProperty
{
public:
    explicit GraphArtifactProperty(const std::string &name = "");
    virtual ~GraphArtifactProperty();

    GraphArtifactProperty(const GraphArtifactProperty &rhs) = default;
    GraphArtifactProperty &operator=(const GraphArtifactProperty &rhs) = default;

    GraphArtifactProperty(GraphArtifactProperty &&rhs) = default;
    GraphArtifactProperty &operator=(GraphArtifactProperty &&rhs) = default;

    std::string getName() const;
    void setName(const std::string &name);

private:
    std::string name;
};

}

#endif // GRAPHARTIFACTPROPERTY_H
