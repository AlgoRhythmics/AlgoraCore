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

#ifndef VERTEXPAIR_H
#define VERTEXPAIR_H

#include "graphartifact.h"

namespace Algora {

class Vertex;

class VertexPair : public GraphArtifact
{
public:
    explicit VertexPair(Vertex *first, Vertex *second, GraphArtifact *parent = nullptr)
        : GraphArtifact(parent), first(first), second(second) {}

    explicit VertexPair(Vertex *first, Vertex *second, id_type id, GraphArtifact *parent = nullptr)
        : GraphArtifact(id, parent), first(first), second(second) {}

    explicit VertexPair(id_type id, GraphArtifact *parent = nullptr)
        : GraphArtifact(id, parent), first(nullptr), second(nullptr) { invalidate(); }

    virtual ~VertexPair() override { }

    // copying
    VertexPair(const VertexPair &other);
    VertexPair &operator=(const VertexPair &other);

    // moving
    VertexPair(VertexPair &&other);
    VertexPair &operator=(VertexPair &&other);

    Vertex *getFirst() const { return first; }
    Vertex *getSecond() const { return second; }

    // GraphArtifact interface
public:
    virtual std::string typeString() const noexcept override {
        return "VertexPair";
    }
    virtual std::string toString() const override;

protected:
    Vertex *first;
    Vertex *second;
};

inline bool operator==(const VertexPair &lhs, const VertexPair &rhs){
    return (lhs.getFirst() == rhs.getFirst()) &&  (lhs.getSecond() == rhs.getSecond());
}
inline bool operator< (const VertexPair &lhs, const VertexPair &rhs){
    return lhs.getFirst() < rhs.getFirst()
            || (lhs.getFirst() == rhs.getFirst() && lhs.getSecond() < rhs.getSecond());
}
inline bool operator!=(const VertexPair &lhs, const VertexPair &rhs){ return !operator==(lhs,rhs);}
inline bool operator> (const VertexPair &lhs, const VertexPair &rhs){ return  operator< (rhs,lhs);}
inline bool operator<=(const VertexPair &lhs, const VertexPair &rhs){ return !operator> (lhs,rhs);}
inline bool operator>=(const VertexPair &lhs, const VertexPair &rhs){ return !operator< (lhs,rhs);}

std::ostream &operator<<(std::ostream &out, const VertexPair *vp);


}

#endif // VERTEXPAIR_H
