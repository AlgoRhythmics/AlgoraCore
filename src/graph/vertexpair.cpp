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

#include "vertexpair.h"
#include "vertex.h"

#include <sstream>

namespace Algora {

VertexPair::VertexPair(const VertexPair &other)
    : GraphArtifact(other), first(other.first), second(other.second) {
    setParent(nullptr);
}

VertexPair &VertexPair::operator=(const VertexPair &other) {
    if (&other == this) {
        return *this;
    }

    GraphArtifact::operator=(other);
    setParent(nullptr);
    first = other.first;
    second = other.second;

    return *this;
}

VertexPair::VertexPair(VertexPair &&other)
    : GraphArtifact(nullptr)
{
    if (other.getParent() != nullptr) {
        // moving illegal
        operator=(other);
    } else {
        operator=(std::move(other));
    }
}

VertexPair &VertexPair::operator=(VertexPair &&other) {
    if (&other == this) {
        return *this;
    }

    if (other.getParent() != nullptr) {
        // use copy
        return operator=(other);
    }
    GraphArtifact::operator=(std::move(other));
    first = other.first;
    second = other.second;

    other.first = nullptr;
    other.second = nullptr;
    other.invalidate();

    return *this;
}


std::string VertexPair::toString() const
{
    std::ostringstream strStream;
    strStream << typeString()
              << " ("
              << first->toString()
              << " -> "
              << second->toString()
              << ") ["
              << idString() << "]";

    return strStream.str();
}

std::ostream &operator<<(std::ostream &out, const VertexPair *vp)
{
    if (!vp) {
        out << "null";
    } else {
        auto name = vp->getName();
        if (name.empty()) {
            name = std::to_string(vp->getId());
        }
        out << "(" << vp->getFirst() << ", " << vp->getSecond() << ")[" << name << "]";
    }
    return out;
}

}
