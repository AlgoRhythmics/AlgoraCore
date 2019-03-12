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

    void mapVertices(const VertexMapping &vvFun) const { mapVerticesUntil(vvFun, vertexFalse); }
    void mapArcs(const ArcMapping &avFun) const { mapArcsUntil(avFun, arcFalse); }
    void mapOutgoingArcs(const Vertex *v, const ArcMapping &avFun) const { mapOutgoingArcsUntil(v, avFun, arcFalse); }
    void mapIncomingArcs(const Vertex *v, const ArcMapping &avFun) const { mapIncomingArcsUntil(v, avFun, arcFalse); }

    virtual void mapVerticesUntil(const VertexMapping &vvFun, const VertexPredicate &breakCondition) const {
        graph->mapVerticesUntil(vvFun, breakCondition); }
    virtual void mapArcsUntil(const ArcMapping &avFun, const ArcPredicate &breakCondition) const {
        graph->mapArcsUntil(avFun, breakCondition); }
    virtual void mapOutgoingArcsUntil(const Vertex *v, const ArcMapping &avFun, const ArcPredicate &breakCondition) const {
        graph->mapOutgoingArcsUntil(v, avFun, breakCondition); }
    virtual void mapIncomingArcsUntil(const Vertex *v, const ArcMapping &avFun, const ArcPredicate &breakCondition) const {
        graph->mapIncomingArcsUntil(v, avFun, breakCondition); }

    virtual std::string getVertexName(const Vertex *v) const { return v->toString(); }

private:
    DiGraph *graph;
};

}

#endif // DIGRAPHINFO_H
