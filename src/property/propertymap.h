/**
 * Copyright (C) 2013 - 2016 : Kathrin Hanauer
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


#ifndef PROPERTYMAP_H
#define PROPERTYMAP_H

#include "property.h"

#include <unordered_map>

namespace Algora {

template<typename T>
class PropertyMap : public Property<T>
{
public:
    PropertyMap(const T &defaultValue, const std::string &name = "")
        : Property<T>(name), defaultValue(defaultValue) { }
    virtual ~PropertyMap() { }

    virtual T getDefaultValue() const { return defaultValue; }

    virtual bool isSetExplicitly(const GraphArtifact *ga) const {
        return map.count(ga) > 0;
    }

    virtual void setValue(const GraphArtifact *ga, const T &value) {
        map[ga] = value;
    }

    virtual void resetToDefault(const GraphArtifact *ga) {
        auto i = map.find(ga);
        if (i != map.end()) { map.erase(i); }
    }

    T &operator[](const GraphArtifact *ga) {
        if (!isSetExplicitly(ga)) {
            map[ga] = defaultValue;
        }
        return map[ga];
    }

    // Property interface
public:
    virtual T getValue(const GraphArtifact *ga) const override {
        return map.count(ga) > 0 ? map.at(ga) : defaultValue;
    }

private:
    T defaultValue;
    typename std::unordered_map<const GraphArtifact*,T> map;
};

}

#endif // PROPERTYMAP_H
