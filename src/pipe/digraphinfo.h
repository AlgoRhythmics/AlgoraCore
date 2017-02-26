// license
#ifndef DIGRAPHINFO_H
#define DIGRAPHINFO_H

#include "graph/digraph.h"

namespace Algora {

class DiGraphInfo
{
public:
    DiGraphInfo(DiGraph *graph) : graph(graph) {}
    virtual ~DiGraphInfo() {}

    DiGraph *getGraph() const { return graph; }

    void mapVertices(VertexMapping vvFun) const { mapVerticesUntil(vvFun, vertexFalse); }
    void mapArcs(ArcMapping avFun) const { mapArcsUntil(avFun, arcFalse); }
    void mapOutgoingArcs(const Vertex *v, ArcMapping avFun) const { mapOutgoingArcsUntil(v, avFun, arcFalse); }
    void mapIncomingArcs(const Vertex *v, ArcMapping avFun) const { mapIncomingArcsUntil(v, avFun, arcFalse); }

    virtual void mapVerticesUntil(VertexMapping vvFun, VertexPredicate breakCondition) const {
        graph->mapVerticesUntil(vvFun, breakCondition); }
    virtual void mapArcsUntil(ArcMapping avFun, ArcPredicate breakCondition) const {
        graph->mapArcsUntil(avFun, breakCondition); }
    virtual void mapOutgoingArcsUntil(const Vertex *v, ArcMapping avFun, ArcPredicate breakCondition) const {
        graph->mapOutgoingArcsUntil(v, avFun, breakCondition); }
    virtual void mapIncomingArcsUntil(const Vertex *v, ArcMapping avFun, ArcPredicate breakCondition) const {
        graph->mapIncomingArcsUntil(v, avFun, breakCondition); }

    virtual std::string getVertexName(const Vertex *v) const { return v->toString(); }

private:
    DiGraph *graph;
};

}

#endif // DIGRAPHINFO_H
