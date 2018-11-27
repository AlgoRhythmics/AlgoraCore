/**
 * Copyright (C) 2013 - 2018 : Kathrin Hanauer
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

#ifndef ARC_H
#define ARC_H

#include "vertexpair.h"

namespace Algora {

class Vertex;

class Arc : public VertexPair
{
    friend class DiGraph;

public:
    virtual ~Arc() {}

    virtual Vertex *getTail() const { return getFirst(); }
    virtual Vertex *getHead() const { return getSecond(); }
    virtual unsigned int getSize() const {
        return 1U;
    }
    bool isLoop() const {
        return getTail() == getHead();
    }

    // GraphArtifact interface
public:
    virtual std::string typeString() const noexcept override {
        return "Arc";
    }

protected:
    explicit Arc(Vertex *tail, Vertex *head, GraphArtifact *parent = 0)
        : VertexPair(tail, head, parent) {}
    explicit Arc(Vertex *tail, Vertex *head, unsigned int id, GraphArtifact *parent = 0)
        : VertexPair(tail, head, id, parent) {}
};

}

#endif // ARC_H
