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

#ifndef MULTIARC_H
#define MULTIARC_H

#include "arc.h"

namespace Algora {

class Vertex;

class MultiArc : public Arc
{
    friend class DiGraph;

public:
    virtual ~MultiArc() override;

    virtual size_type getSize() const override = 0;

    // GraphArtifact interface
public:
    virtual std::string typeString() const noexcept override {
        return "MultiArc";
    }
    virtual std::string toString() const override;

protected:
    explicit MultiArc(Vertex *tail, Vertex *head, GraphArtifact *parent = nullptr);
    explicit MultiArc(Vertex *tail, Vertex *head, id_type id, GraphArtifact *parent = nullptr);
};

}

#endif // MULTIARC_H
