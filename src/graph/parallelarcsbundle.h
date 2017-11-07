/**
 * Copyright (C) 2013 - 2017 : Kathrin Hanauer
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

#include "multiarc.h"
#include "graph_functional.h"
#include "graph.visitor/arcvisitor.h"

#include <vector>

namespace Algora {

class ParallelArcsBundle : public MultiArc
{

public:
    explicit ParallelArcsBundle(Vertex *tail, Vertex *head, GraphArtifact *parent = 0);
    explicit ParallelArcsBundle(Arc *arc);

    virtual ~ParallelArcsBundle();

    virtual void getArcs(std::vector<Arc*> *l) const;
    virtual void acceptArcVisitor(ArcVisitor *aVisitor) const {
        mapArcs(aVisitor->getVisitorFunction());
    }
    virtual void mapArcs(ArcMapping am) const {
        mapArcsUntil(am, arcFalse);
    }
    virtual void mapArcsUntil(ArcMapping am, ArcPredicate ap) const;

    virtual bool addArc(Arc *a);
    virtual void removeArc(Arc *a);
    virtual bool containsArc(Arc *a) const;
    virtual void clear();

    // MultiArc interface
public:
    virtual int getSize() const override;

    // GraphArtifact interface
public:
    virtual std::string typeString() const override {
        return "Parallel Arcs Bundle";
    }
    virtual std::string toString() const override;

private:
    struct CheshireCat;
    CheshireCat *grin;
};

}

#endif // PARALLELARCSBUNDLE_H
