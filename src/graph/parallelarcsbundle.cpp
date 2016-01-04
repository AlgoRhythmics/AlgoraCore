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


#include "parallelarcsbundle.h"

#include "vertex.h"
#include "graph.visitor/arcvisitor.h"

#include <algorithm>
#include <sstream>

namespace Algora {

struct ParallelArcsBundle::CheshireCat {
    std::vector<Arc*> arcsBundle;
};

ParallelArcsBundle::ParallelArcsBundle(Vertex *tail, Vertex *head, GraphArtifact *parent)
    : Arc(tail, head, parent)
{
    grin = new CheshireCat;
}

ParallelArcsBundle::~ParallelArcsBundle()
{
    for (auto i = grin->arcsBundle.cbegin();
         i != grin->arcsBundle.cend(); i++) {
        delete *i;
    }
    delete grin;
}

void ParallelArcsBundle::getArcs(std::vector<Arc *> *l) const
{
    std::copy(grin->arcsBundle.cbegin(), grin->arcsBundle.cend(), std::back_inserter(*l));
}

int ParallelArcsBundle::getSize() const
{
    return grin->arcsBundle.size();
}

void ParallelArcsBundle::acceptArcVisitor(ArcVisitor *aVisitor) const
{
    for (auto i = grin->arcsBundle.cbegin();
         i != grin->arcsBundle.cend(); i++) {
        aVisitor->visitArc(*i);
    }
}

bool ParallelArcsBundle::addArc(Arc *a)
{
    if (a->getTail() != getTail() || a->getHead() != getHead()) {
        return false;
    }

    grin->arcsBundle.push_back(a);
    return true;
}

void ParallelArcsBundle::removeArc(Arc *a)
{
    grin->arcsBundle.erase(std::remove(grin->arcsBundle.begin(), grin->arcsBundle.end(), a), grin->arcsBundle.end());
}

bool ParallelArcsBundle::containsArc(Arc *a) const
{
    auto i = std::find(grin->arcsBundle.cbegin(), grin->arcsBundle.cend(), a);
    return i != grin->arcsBundle.end();
}


std::string ParallelArcsBundle::toString() const
{
    std::ostringstream strStream;
    strStream << "[Parallel Arcs Bundle #"
        << std::to_string(getId())
        << ": "
        << getTail()->toString()
        << " -> "
        << getHead()->toString()
        << ", size "
        << grin->arcsBundle.size()
        << "]";

    return strStream.str();
}

}

