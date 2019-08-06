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

#ifndef INCIDENCELISTVERTEX_H
#define INCIDENCELISTVERTEX_H

#include "graph/vertex.h"
#include "graph/graph_functional.h"

namespace Algora {

class IncidenceListGraph;
class Arc;
class ArcVisitor;

template<typename T>
class FastPropertyMap;

class IncidenceListVertex : public Vertex
{
    friend class IncidenceListGraph;
    friend class IncidenceListGraphImplementation;
    friend class SuperDiGraph;

public:
    explicit IncidenceListVertex(id_type id,
                                 FastPropertyMap<size_type> &sharedOutIndex,
                                 FastPropertyMap<size_type> &sharedInIndex,
                                 GraphArtifact *parent = nullptr, size_type index = 0);
    virtual ~IncidenceListVertex();

    // disable copying and moving
    IncidenceListVertex(const IncidenceListVertex &other) = delete;
    IncidenceListVertex &operator=(const IncidenceListVertex &other) = delete;
    IncidenceListVertex(IncidenceListVertex &&other) = delete;
    IncidenceListVertex &operator=(IncidenceListVertex &&other) = delete;

    virtual bool hasOutgoingArc(const Arc *a) const;
    virtual bool hasIncomingArc(const Arc *a) const;
    virtual Arc *outgoingArcAt(size_type i, bool multiArcsAsSimple = false) const;
    virtual Arc *incomingArcAt(size_type i, bool multiArcsAsSimple = false) const;

    size_type outIndexOf(const Arc *a) const;
    size_type inIndexOf(const Arc *a) const;

    void acceptOutgoingArcVisitor(ArcVisitor *aVisitor) const;
    void acceptIncomingArcVisitor(ArcVisitor *aVisitor) const;
    virtual bool mapOutgoingArcs(const ArcMapping &avFun, const ArcPredicate &breakCondition = arcFalse, bool checkValidity = true) const;
    virtual bool mapIncomingArcs(const ArcMapping &avFun, const ArcPredicate &breakCondition = arcFalse, bool checkValidity = true) const;

    size_type getOutDegree(bool multiArcsAsSimple = false) const;
    size_type getInDegree(bool multiArcsAsSimple = false) const;
    bool isSource() const;
    bool isSink() const;
    bool isIsolated() const { return isSource() && isSink(); }

    size_type getIndex() const;

protected:
    virtual void addOutgoingArc(Arc *a);
    virtual void removeOutgoingArc(const Arc *a);
    virtual void clearOutgoingArcs();

    virtual void addIncomingArc(Arc *a);
    virtual void removeIncomingArc(const Arc *a);
    virtual void clearIncomingArcs();

    virtual void enableConsistencyCheck(bool enable);

    void setIndex(size_type i);

    void hibernate();
    void recycle();

private:
    class CheshireCat;
    CheshireCat *grin;
};

}

#endif // INCIDENCELISTVERTEX_H
