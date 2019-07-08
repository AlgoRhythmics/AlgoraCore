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

#ifndef VERTEX_H
#define VERTEX_H

#include "graphartifact.h"

namespace Algora {

class Vertex : public GraphArtifact
{
    friend class Graph;
    friend std::ostream &operator<<(std::ostream &, const Vertex *);

public:
    explicit Vertex(GraphArtifact *parent = nullptr);
    explicit Vertex(id_type id, GraphArtifact *parent = nullptr);
    virtual ~Vertex() override;

    // copying
    Vertex(const Vertex &other) : GraphArtifact(other) { setParent(nullptr); }
    Vertex &operator=(const Vertex &other);
    // moving
    Vertex(Vertex &&other);
    Vertex &operator=(Vertex &&other);

    virtual std::string toString() const override;

protected:

    // GraphArtifact interface
public:
    virtual std::string typeString() const noexcept override { return "Vertex"; }
};

std::ostream &operator<<(std::ostream &out, const Vertex *v);

}

#endif // VERTEX_H
