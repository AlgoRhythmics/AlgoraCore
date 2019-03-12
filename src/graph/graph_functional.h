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

#ifndef GRAPH_FUNCTIONAL_H
#define GRAPH_FUNCTIONAL_H

#include <functional>

namespace Algora {

class Vertex;
class Arc;

typedef std::function<void(Vertex *v)> VertexMapping;
typedef std::function<void(Arc *a)> ArcMapping;

typedef std::function<void(const Vertex *v)> ConstVertexMapping;
typedef std::function<void(const Arc *a)> ConstArcMapping;

typedef std::function<bool(const Vertex *v)> VertexPredicate;
typedef std::function<bool(const Arc *v)> ArcPredicate;

extern const VertexMapping vertexNothing;
extern const ArcMapping arcNothing;

extern const VertexPredicate vertexTrue;
extern const VertexPredicate vertexFalse;

extern const ArcPredicate arcTrue;
extern const ArcPredicate arcFalse;

}

#endif // GRAPH_FUNCTIONAL_H

