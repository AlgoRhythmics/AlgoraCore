/**
 * Copyright (C) 2013 - 2018 : Kathrin Hanauer
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

#ifndef INCIDENCELISTVERTEX_H
#define INCIDENCELISTVERTEX_H

#include "graph/vertex.h"
#include "graph/graph_functional.h"

namespace Algora {

class IncidenceListGraph;
class Arc;
class ArcVisitor;

class IncidenceListVertex : public Vertex
{
    friend class IncidenceListGraph;
    friend class IncidenceListGraphImplementation;

public:
    virtual bool hasOutgoingArc(const Arc *a) const;
    virtual bool hasIncomingArc(const Arc *a) const;
    virtual Arc *outgoingArcAt(unsigned int i, bool multiArcsAsSimple = false) const;
    virtual Arc *incomingArcAt(unsigned int i, bool multiArcsAsSimple = false) const;

    void acceptOutgoingArcVisitor(ArcVisitor *aVisitor) const;
    void acceptIncomingArcVisitor(ArcVisitor *aVisitor) const;
    virtual bool mapOutgoingArcs(ArcMapping avFun, ArcPredicate breakCondition = arcFalse, bool checkValidity = true) const;
    virtual bool mapIncomingArcs(ArcMapping avFun, ArcPredicate breakCondition = arcFalse, bool checkValidity = true) const;

    int getOutDegree(bool multiArcsAsSimple = false) const;
    int getInDegree(bool multiArcsAsSimple = false) const;
    bool isSource() const;
    bool isSink() const;
    bool isIsolated() const { return isSource() && isSink(); }

    int getIndex() const;

protected:
    explicit IncidenceListVertex(unsigned int id, GraphArtifact *parent = 0, int index = -1);
    virtual ~IncidenceListVertex();

    virtual void addOutgoingArc(Arc *a);
    virtual void removeOutgoingArc(const Arc *a);
    virtual void clearOutgoingArcs();

    virtual void addIncomingArc(Arc *a);
    virtual void removeIncomingArc(const Arc *a);
    virtual void clearIncomingArcs();

    virtual void enableConsistencyCheck(bool enable);

    void setIndex(int i);

private:
    class CheshireCat;
    CheshireCat *grin;
};

}

#endif // INCIDENCELISTVERTEX_H
