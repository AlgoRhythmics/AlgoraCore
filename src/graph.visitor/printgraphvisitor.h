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

#ifndef PRINTGRAPHVISITOR_H
#define PRINTGRAPHVISITOR_H

#include "graphvisitor.h"

#include <iostream>

namespace Algora {

class PrintGraphVisitor : public GraphVisitor
{
public:
    explicit PrintGraphVisitor(const std::string &sep = std::string(), std::ostream &os = std::cout);
    virtual ~PrintGraphVisitor();

    void setItemSeparator(const std::string &sep);

    void reset();

    // GraphVisitor interface
public:
    virtual void visitArtifact(GraphArtifact *artifact) override;

private:
    struct CheshireCat;
    CheshireCat *cat;

};

}

#endif // PRINTGRAPHVISITOR_H
