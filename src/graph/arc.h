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

#ifndef ARC_H
#define ARC_H

#include "vertexpair.h"

namespace Algora {

class Vertex;

class Arc : public VertexPair
{
    friend class DiGraph;
    friend class IncidenceListGraphImplementation;

public:
    explicit Arc(Vertex *tail, Vertex *head, GraphArtifact *parent = nullptr)
        : VertexPair(tail, head, parent) {}
    explicit Arc(Vertex *tail, Vertex *head, id_type id, GraphArtifact *parent = nullptr)
        : VertexPair(tail, head, id, parent) {}
    explicit Arc(id_type id, GraphArtifact *parent = nullptr)
        : VertexPair(id, parent) {}

    virtual ~Arc() override {}

    // copying
    Arc(const Arc &other) = default;
    Arc &operator=(const Arc &other) = default;
    // moving
    Arc(Arc &&other) = default;
    Arc &operator=(Arc &&other) = default;

    virtual Vertex *getTail() const { return getFirst(); }
    virtual Vertex *getHead() const { return getSecond(); }
    virtual size_type getSize() const {
        return 1ULL;
    }
    bool isLoop() const {
        return getTail() == getHead();
    }

    // GraphArtifact interface
public:
    virtual std::string typeString() const noexcept override;

protected:
    void hibernate() { invalidate(); }
    void recycle(Vertex *tail, Vertex *head) {
        first = tail;
        second = head;
        revalidate();
    }
};

}

#endif // ARC_H
