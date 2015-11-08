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


#ifndef PRINTGRAPHVISITOR_H
#define PRINTGRAPHVISITOR_H

#include "vertexvisitor.h"
#include "arcvisitor.h"

namespace Algora {

class PrintGraphVisitor : public VertexVisitor, public ArcVisitor
{
public:
    explicit PrintGraphVisitor();

    // ArcVisitor interface
public:
    virtual void visitArc(Arc *a);

    // VertexVisitor interface
public:
    virtual void visitVertex(Vertex *v);

};

}

#endif // PRINTGRAPHVISITOR_H
