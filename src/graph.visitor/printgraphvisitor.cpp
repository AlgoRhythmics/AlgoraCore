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

#include "printgraphvisitor.h"

#include "graph/graphartifact.h"

#include <iostream>

namespace Algora {

struct PrintGraphVisitor::CheshireCat {
    bool firstItemVisited;
    std::string itemSeparator;
    std::ostream &out;

    CheshireCat(const std::string &sep, std::ostream &os) :
        firstItemVisited(false), itemSeparator(sep), out(os) { }
};

PrintGraphVisitor::PrintGraphVisitor(const std::string &sep, std::ostream &os)
 : cat(new CheshireCat(sep, os))
{

}

PrintGraphVisitor::~PrintGraphVisitor()
{
    delete cat;
}

void PrintGraphVisitor::setItemSeparator(const std::string &sep)
{
    cat->itemSeparator = sep;
    reset();
}

void PrintGraphVisitor::reset()
{
    cat->firstItemVisited = false;
}

void PrintGraphVisitor::visitArtifact(GraphArtifact *artifact)
{
    if (cat->firstItemVisited) {
        cat->out << cat->itemSeparator;
    } else {
        cat->firstItemVisited = true;
    }
    cat->out << artifact->toString();
}

}
