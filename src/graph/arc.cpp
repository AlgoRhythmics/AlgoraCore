/**
 * Copyright (C) 2013 - 2015 : Kathrin Hanauer
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


#include "arc.h"
#include "vertex.h"

#include <sstream>

using namespace Algora;

struct Arc::CheshireCat {
    Vertex *tail;
    Vertex *head;

    CheshireCat(Vertex *tail, Vertex *head)
        : tail(tail), head(head)
    { }
};

Arc::Arc(Vertex *tail, Vertex *head, GraphArtifact *parent)
    : GraphArtifact(parent)
{
    cat = new CheshireCat(tail, head);
}

Arc::~Arc()
{
    delete cat;
}

Vertex *Arc::getTail() const
{
    return cat->tail;
}

Vertex *Arc::getHead() const
{
    return cat->head;
}

std::string Arc::toString()
{
    std::ostringstream strStream;
    strStream << "[Arc #"
        << std::to_string(getId())
        << ": "
        << cat->tail->toString()
        << " -> "
        << cat->head->toString()
        << "]";

    return strStream.str();
}
