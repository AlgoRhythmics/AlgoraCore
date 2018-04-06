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

#include "weightedarc.h"

namespace Algora {

struct WeightedArc::CheshireCat {
    int weight;

    explicit CheshireCat(int w = 0) : weight(w) { }
};

WeightedArc::WeightedArc(Vertex *tail, Vertex *head, int weight, GraphArtifact *parent)
    : MultiArc(tail, head, parent), grin(new CheshireCat(weight))
{

}

unsigned int WeightedArc::getSize() const
{
    return grin->weight;
}

WeightedArc::~WeightedArc()
{
    delete grin;
}

void WeightedArc::setWeight(int weight)
{
    grin->weight = weight;
}


}
