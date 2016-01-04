/**
 * Copyright (C) 2013 - 2015 : Kathrin Hanauer
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

#include "../graph/vertex.h"

namespace Algora {

class IncidenceListGraph;
class Arc;
class ArcVisitor;

class IncidenceListVertex : public Vertex
{
    friend class IncidenceListGraph;

protected:
    explicit IncidenceListVertex(IncidenceListGraph *graph = 0);
    virtual ~IncidenceListVertex();

    virtual int getOutDegree() const;
    virtual int getInDegree() const;

    virtual void addOutgoingArc(Arc *a);
    virtual void removeOutgoingArc(Arc *a);
    virtual void clearOutgoingArcs();

    virtual void addIncomingArc(Arc *a);
    virtual void removeIncomingArc(Arc *a);
    virtual void clearIncomingArcs();

    virtual void acceptOutgoingArcVisitor(ArcVisitor *aVisitor) const;
    virtual void acceptIncomingArcVisitor(ArcVisitor *aVisitor) const;

private:
    class CheshireCat;
    CheshireCat *grin;
};

}

#endif // INCIDENCELISTVERTEX_H
