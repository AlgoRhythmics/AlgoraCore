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


#include "printgraphvisitor.h"

#include "graph/vertex.h"
#include "graph/arc.h"

#include <iostream>

using namespace Algora;

struct PrintGraphVisitor::CheshireCat {
    bool endLine;
    std::ostream &out;

    CheshireCat() :
        endLine(false), out(std::cout) { }
};

PrintGraphVisitor::PrintGraphVisitor()
 : cat(new CheshireCat)
{

}

PrintGraphVisitor::~PrintGraphVisitor()
{
    delete cat;
}

void PrintGraphVisitor::setEndLineAfterEachElement(bool endl)
{
    cat->endLine = endl;
}

void PrintGraphVisitor::visitArc(Arc *a)
{
    cat->out << a->toString();
    if (cat->endLine) {
        cat->out << std::endl;
    }
}

void PrintGraphVisitor::visitVertex(Vertex *v)
{
    cat->out << v->toString();
    if (cat->endLine) {
        cat->out << std::endl;
    }
}
