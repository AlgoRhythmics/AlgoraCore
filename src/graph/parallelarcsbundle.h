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


#ifndef PARALLELARCSBUNDLE_H
#define PARALLELARCSBUNDLE_H

#include "arc.h"
#include <vector>

namespace Algora {

class ArcVisitor;

class ParallelArcsBundle : public Arc
{
public:
    explicit ParallelArcsBundle(Vertex *tail, Vertex *head, GraphArtifact *parent = 0);
    explicit ParallelArcsBundle(Arc *arc);
    virtual ~ParallelArcsBundle();

    virtual void getArcs(std::vector<Arc*> *l) const;
    virtual int getSize() const;
    virtual void acceptArcVisitor(ArcVisitor *aVisitor) const;

    virtual bool addArc(Arc *a);
    virtual void removeArc(Arc *a);
    virtual bool containsArc(Arc *a) const;

    // GraphArtifact interface
public:
    virtual std::string toString() const override;

private:
    struct CheshireCat;
    CheshireCat *grin;

};

}

#endif // PARALLELARCSBUNDLE_H
