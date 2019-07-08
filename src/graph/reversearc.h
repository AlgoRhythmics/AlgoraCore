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

#ifndef REVERSEARC_H
#define REVERSEARC_H

#include "arc.h"

namespace Algora {

class ReverseArc : public Arc
{
public:
    ReverseArc(Arc *a) : Arc(a->getHead(), a->getTail(), a->getParent()), arc(a) { }
    virtual ~ReverseArc() override {}

    // GraphArtifact interface
public:
    virtual std::string typeString() const noexcept override { return arc->typeString() + " [reversed]"; }
    virtual std::string toString() const override { return arc->toString() + "^R"; }

    // Arc interface
public:
    virtual size_type getSize() const override { return arc->getSize(); }

private:
    Arc *arc;
};

}

#endif // REVERSEARC_H
