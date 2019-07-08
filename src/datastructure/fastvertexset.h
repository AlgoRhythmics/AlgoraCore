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

#ifndef FASTVERTEXSET_H
#define FASTVERTEXSET_H

#include "graph/vertex.h"
#include "property/fastpropertymap.h"

#include <vector>

namespace Algora {

class FastVertexSet
{
public:
    typedef Vertex* value_type;
    typedef std::vector<value_type> container;
    typedef container::const_iterator const_iterator;
    typedef std::vector<value_type>::size_type size_type;

    explicit FastVertexSet(size_type capacity = 0ULL) {
        setIndex.setDefaultValue(0U);
        if (capacity > 0U) {
            setIndex.resetAll(capacity);
        }
    }

    bool contains(const Vertex *v) {
        return setIndex(v) > 0U;
    }

    void add(Vertex *v) {
        if (setIndex(v) > 0U) {
            return;
        }
        set.push_back(v);
        setIndex[v] = set.size();
    }

    void remove(Vertex *v) {
        if (setIndex(v) == 0U) {
            return;
        }
        auto index = setIndex(v) - 1U;
        set[index] = set.back();
        set.pop_back();
        setIndex[set[index]] = index + 1U;
        setIndex.resetToDefault(v);
    }

    void clear() {
        set.clear();
        setIndex.resetAll();
    }

    size_type getSize() const {
        return set.size();
    }

    const_iterator cbegin() const {
        return set.cbegin();
    }

    const_iterator cend() const {
        return set.cend();
    }

private:
    std::vector<Vertex*> set;
    FastPropertyMap<size_type> setIndex;
};

}
#endif // FASTVERTEXSET_H
