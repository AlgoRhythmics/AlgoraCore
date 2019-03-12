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

#ifndef GRAPHPROCESSOR_H
#define GRAPHPROCESSOR_H

namespace Algora {

class DiGraph;
class DiGraphInfo;

class DiGraphProcessor
{
public:
    DiGraphProcessor() { }
    virtual ~DiGraphProcessor() { }

    virtual void processGraph(const DiGraph *graph, const DiGraphInfo *info = 0) = 0;
};

}

#endif // GRAPHPROCESSOR_H
