/**
 * Copyright (C) 2013 - 2020 : Kathrin Hanauer
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

#include "graph.incidencelist/incidencelistgraph.h"
#include "algorithm.basic.traversal/breadthfirstsearch.h"
#include "property/fastpropertymap.h"

#include <iostream>

int main(int argc, char *argv[])
{
	using namespace Algora;

	// First, we create a graph.
	// Calling setName() is optional, we only use it here to get a nicer output.
	IncidenceListGraph g;

	auto u = g.addVertex();
	u->setName("u");
	auto v = g.addVertex();
	v->setName("v");
	auto w = g.addVertex();
	w->setName("w");
	auto x = g.addVertex();
	x->setName("x");
	auto y = g.addVertex();
	y->setName("y");
	auto z = g.addVertex();
	z->setName("z");

	g.addArc(u, v)->setName("u -> v");
	g.addArc(u, y)->setName("u -> y");
	g.addArc(v, w)->setName("v -> w");
	g.addArc(v, x)->setName("v -> x, 1");
	g.addArc(v, x)->setName("v -> x, 2");
	g.addArc(x, u)->setName("x -> u");
	g.addArc(y, z)->setName("y -> z");
	g.addArc(z, w)->setName("z -> w");

	// Configure the breadth-first search algorithm:
	BreadthFirstSearch<FastPropertyMap> bfs;
	bfs.setGraph(&g);
	bfs.setStartVertex(u);
	// We want it to store the BFS number of each vertex in this map:
	FastPropertyMap<DiGraph::size_type> bfsNum(0);
	bfs.useModifiableProperty(&bfsNum);

	// We want to be notified whenever the algorithm discovers a new tree arc:
	bfs.onTreeArcDiscover([](Arc *a) { std::cout << "Discovered tree arc " << a << std::endl; });

	// prepare() checks the algorithm's preconditions:
	if (!bfs.prepare()) {
		std::cerr << "Could not prepare BFS algorithm. Please check the preconditions." << std::endl;
		return 1;
	}

	// run() runs the algorithm, whereas deliver() only returns the result computed during run()
	std::cout << "Running breadth-first search..." << std::endl;
	bfs.run();
	auto numReachedVertices = bfs.deliver();
	std::cout << "Done, reached " << numReachedVertices << " vertices." << std::endl;

	// Finally, output the BFS number for each vertex:
	g.mapVertices([&bfsNum](Vertex *v) { std::cout << "BFS number of " << v << ": " << bfsNum(v) << std::endl; });

	return 0;
}
