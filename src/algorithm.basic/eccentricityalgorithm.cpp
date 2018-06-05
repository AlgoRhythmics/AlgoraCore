#include "eccentricityalgorithm.h"
#include "breadthfirstsearch.h"

#include "graph/digraph.h"
#include "algorithm/digraphalgorithmexception.h"

#include <climits>

namespace Algora {

const int EccentricityAlgorithm::INFINITE = INT_MAX;

EccentricityAlgorithm::EccentricityAlgorithm()
    : ValueComputingAlgorithm<int>(), vertex(nullptr), eccentricity(INFINITE)
{

}

bool EccentricityAlgorithm::prepare()
{
    return ValueComputingAlgorithm<int>::prepare() && diGraph->containsVertex(vertex);
}

void EccentricityAlgorithm::run()
{
    BreadthFirstSearch bfs(false);
    bfs.setStartVertex(vertex);
    bfs.orderAsValues(false);
    eccentricity = runAlgorithm(bfs, diGraph) ? bfs.getMaxLevel() :  INFINITE;
}

void EccentricityAlgorithm::onDiGraphSet() {
    ValueComputingAlgorithm<int>::onDiGraphSet();
    eccentricity = -1;
}

}
