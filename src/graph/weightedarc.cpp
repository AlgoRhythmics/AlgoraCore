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

#include "weightedarc.h"

namespace Algora {

WeightedArc::WeightedArc(Vertex *tail, Vertex *head, weight_type weight, GraphArtifact *parent)
    : MultiArc(tail, head, parent), weight(weight)
{

}

WeightedArc::WeightedArc(Vertex *tail, Vertex *head, weight_type weight, id_type id, GraphArtifact *parent)
    : MultiArc(tail, head, id, parent), weight(weight)
{
}

GraphArtifact::size_type WeightedArc::getSize() const
{
    return weight;
}

WeightedArc::~WeightedArc()
{
}

void WeightedArc::setWeight(weight_type w)
{
    this->weight = w;
}


}
