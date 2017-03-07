// license
#include "accessibilityalgorithm.h"

#include "property/propertymap.h"
#include "graph/digraph.h"
#include "graph/vertex.h"
#include "graph/arc.h"
#include "breadthfirstsearch.h"
#include "algorithm/digraphalgorithmexception.h"

#include <boost/logic/tribool.hpp>

namespace Algora {

typedef boost::logic::tribool TriBool;

struct AccessibilityAlgorithm::CheshireCat {
    PropertyMap<PropertyMap<TriBool>> isAccessible;


    CheshireCat() {
        TriBool unknown(boost::logic::indeterminate);
        PropertyMap<TriBool> allUnknown(unknown);
        isAccessible.setDefaultValue(allUnknown);
    }

    bool checkAccessibility(DiGraph *graph, Vertex *source, Vertex *target);
};

AccessibilityAlgorithm::AccessibilityAlgorithm(bool computeValues)
    : PropertyComputingAlgorithm<void, bool>(computeValues), grin(new CheshireCat)
{

}

AccessibilityAlgorithm::~AccessibilityAlgorithm()
{
    delete grin;
}

bool AccessibilityAlgorithm::canAccess(Vertex *source, Vertex *target)
{
    TriBool accessible(grin->isAccessible[source][target]);
    if (!boost::logic::indeterminate(accessible)) {
        return accessible;
    }
    return grin->checkAccessibility(diGraph, source, target);
}

void AccessibilityAlgorithm::run()
{

}

void AccessibilityAlgorithm::onDiGraphSet()
{
    grin->isAccessible.resetAll();
    diGraph->mapVertices([&](Vertex *v) {
        grin->isAccessible[v][v] = true;
    });
    diGraph->mapArcs([&](Arc *a) {
        grin->isAccessible[a->getTail()][a->getHead()] = true;
    });
}

bool AccessibilityAlgorithm::CheshireCat::checkAccessibility(DiGraph *graph, Vertex *source, Vertex *target)
{
    BreadthFirstSearch bfs(false);
    bfs.setGraph(graph);

    bfs.setStartVertex(source);
    bool pathFound = false;
    bfs.setVertexStopCondition([&](const Vertex *) { return pathFound; });
    bfs.onVertexDiscover([&](const Vertex *v) {
        isAccessible[source][v] = true;
        TriBool accessible(isAccessible[v][target]);
        if (accessible) {
            pathFound = true;
        } else if (!accessible) {
            return false;
        }
        return true;
    });

    if (!bfs.prepare()) {
        throw DiGraphAlgorithmException("Could not prepare BFS algorithm.");
    }
    bfs.run();
    bfs.deliver();

    return pathFound;
}

}
