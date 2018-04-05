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

#ifndef WEIGHTEDARC_H
#define WEIGHTEDARC_H

#include "multiarc.h"

namespace Algora {

class WeightedArc : public MultiArc
{
    friend class DiGraph;

public:
    virtual ~WeightedArc();

    int getWeight() const {
        return getSize();
    }
    void setWeight(int weight);

protected:
    explicit WeightedArc(Vertex *tail, Vertex *head, int weight, GraphArtifact *parent = 0);

private:
    struct CheshireCat;
    CheshireCat *grin;

    // GraphArtifact interface
public:
    virtual std::string typeString() const override {
        return "Weighted Arc";
    }

    // MultiArc interface
public:
    virtual int getSize() const override;
};

}

#endif // WEIGHTEDARC_H
