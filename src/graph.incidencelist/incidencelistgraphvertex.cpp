#include "incidencelistgraphvertex.h"

#include "incidencelistgraph.h"
#include "graph/arc.h"
#include "graph.visitor/arcvisitor.h"

#include <vector>
#include <stdexcept>
#include <algorithm>

namespace Algora {

typedef std::vector<Arc*> ArcList;

void removeArcFromList(ArcList &list, const Arc *a);

class IncidenceListGraphVertex::CheshireCat {
public:
    ArcList outgoingArcs;
    ArcList incomingArcs;
};

IncidenceListGraphVertex::IncidenceListGraphVertex(IncidenceListGraph *graph)
    : Vertex(graph), grin(new CheshireCat)
{

}

IncidenceListGraphVertex::~IncidenceListGraphVertex()
{
    delete grin;
}

int IncidenceListGraphVertex::getOutDegree() const
{
    return grin->outgoingArcs.size();
}

void IncidenceListGraphVertex::addOutgoingArc(const Arc *a)
{
    if (a->getTail() != this) {
        throw std::invalid_argument("Arc has other tail.");
    }
    grin->outgoingArcs.push_back(const_cast<Arc*>(a));
}

void IncidenceListGraphVertex::removeOutgoingArc(const Arc *a)
{
    if (a->getTail() != this) {
        throw std::invalid_argument("Arc has other tail.");
    }
    removeArcFromList(grin->outgoingArcs, a);
}

void IncidenceListGraphVertex::clearOutgoingArcs()
{
    grin->outgoingArcs.clear();
}

int IncidenceListGraphVertex::getInDegree() const
{
    return grin->incomingArcs.size();
}

void IncidenceListGraphVertex::addIncomingArc(const Arc *a)
{
    if (a->getHead() != this) {
        throw std::invalid_argument("Arc has other head.");
    }
    grin->incomingArcs.push_back(const_cast<Arc*>(a));
}

void IncidenceListGraphVertex::removeIncomingArc(const Arc *a)
{
    if (a->getHead() != this) {
        throw std::invalid_argument("Arc has other head.");
    }
    removeArcFromList(grin->incomingArcs, a);
}

void IncidenceListGraphVertex::clearIncomingArcs()
{
    grin->incomingArcs.clear();
}

void IncidenceListGraphVertex::acceptOutgoingArcVisitor(ArcVisitor *aVisitor)
{
    for (Arc *a : grin->outgoingArcs) {
        aVisitor->visitArc(a);
    }
}

void IncidenceListGraphVertex::acceptIncomingArcVisitor(ArcVisitor *aVisitor)
{
    for (Arc *a : grin->incomingArcs) {
        aVisitor->visitArc(a);
    }
}

void removeArcFromList(ArcList &list, const Arc *a) {
    list.erase(std::find(std::begin(list), std::end(list), const_cast<Arc*>(a)));
}

}
