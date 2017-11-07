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

#ifndef PROPERTYMAP_H
#define PROPERTYMAP_H

#include "property.h"

//#include <unordered_map>
#include <map>

namespace Algora {

template<typename T>
class PropertyMap : public Property<T>
{
public:
    typedef typename std::map<const GraphArtifact*,T>::iterator iterator;
    typedef typename std::map<const GraphArtifact*,T>::const_iterator const_iterator;

    PropertyMap(const T &defaultValue = T(), const std::string &name = "")
        : Property<T>(name), defaultValue(defaultValue) { }
    PropertyMap(const PropertyMap<T> &other)
        : Property<T>(other),
          defaultValue(other.defaultValue),
          map(other.map) { }
    virtual ~PropertyMap() { }

    PropertyMap &operator=(const PropertyMap<T> &rhs) {
        if (this == &rhs) {
            return *this;
        }
        Property<T>::operator=(rhs);
        defaultValue = rhs.defaultValue;
        map = rhs.map;
        return *this;
    }

    T getDefaultValue() const { return defaultValue; }
    void setDefaultValue(const T &val) { defaultValue = val; }

    bool isSetExplicitly(const GraphArtifact *ga) const {
        return map.count(ga) > 0;
    }

    void setValue(const GraphArtifact *ga, const T &value) {
        map[ga] = value;
    }

    void resetToDefault(const GraphArtifact *ga) {
        auto i = map.find(ga);
        if (i != map.end()) { map.erase(i); }
    }

    void resetAll() {
        map.clear();
    }

    T &operator[](const GraphArtifact *ga) {
        if (!isSetExplicitly(ga)) {
            map[ga] = defaultValue;
        }
        return map[ga];
    }

    // iterators
    iterator begin() {
        return map.begin();
    }

    const_iterator begin() const {
        return map.begin();
    }

    iterator end() {
        return map.end();
    }

    const_iterator end() const {
        return map.end();
    }

    const_iterator cbegin() const noexcept {
        return map.cbegin();
    }
    const_iterator cend() const noexcept {
        return map.cend();
    }

    // Property interface
public:
    virtual T getValue(const GraphArtifact *ga) const override {
        return map.count(ga) > 0 ? map.at(ga) : defaultValue;
    }

private:
    T defaultValue;
    //typename std::unordered_map<const GraphArtifact*,T> map;
    typename std::map<const GraphArtifact*,T> map;
};

}

#endif // PROPERTYMAP_H
