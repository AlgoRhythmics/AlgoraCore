#ifndef INCIDENCELISTGRAPHVERTEX_H
#define INCIDENCELISTGRAPHVERTEX_H

#include "../graph/vertex.h"

namespace Algora {

class IncidenceListGraph;
class Arc;
class ArcVisitor;

class IncidenceListGraphVertex : public Vertex
{
    friend class IncidenceListGraph;

protected:
    explicit IncidenceListGraphVertex(IncidenceListGraph *graph = 0);
    virtual ~IncidenceListGraphVertex();

    virtual int getOutDegree() const;
    virtual int getInDegree() const;

    virtual void addOutgoingArc(const Arc *a);
    virtual void removeOutgoingArc(const Arc *a);
    virtual void clearOutgoingArcs();

    virtual void addIncomingArc(const Arc *a);
    virtual void removeIncomingArc(const Arc *a);
    virtual void clearIncomingArcs();

    virtual void acceptOutgoingArcVisitor(ArcVisitor *aVisitor);
    virtual void acceptIncomingArcVisitor(ArcVisitor *aVisitor);

private:
    class CheshireCat;
    CheshireCat *grin;
};

}

#endif // INCIDENCELISTGRAPHVERTEX_H
