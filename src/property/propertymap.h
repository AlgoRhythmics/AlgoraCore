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

#ifndef PROPERTYMAP_H
#define PROPERTYMAP_H

#include "modifiableproperty.h"
#include "observable.h"

#include <unordered_map>
#include <cmath>
//#include <map>

namespace Algora {

template<typename Tval>
struct PointerHash {
    size_t operator()(const Tval* val) const {
        static const size_t shift = static_cast<size_t>(log2(1 + sizeof(Tval)));
        return reinterpret_cast<size_t>(val) >> shift;
    }
};

template<typename T>
class PropertyMap : public ModifiableProperty<T>
{
public:
    typedef typename std::unordered_map<const GraphArtifact*,T, PointerHash<const GraphArtifact> > MapType;
    //typedef typename std::map<const GraphArtifact*,T> MapType;
    typedef typename MapType::iterator iterator;
    typedef typename MapType::const_iterator const_iterator;

    PropertyMap(const T &defaultValue = T(), const std::string &name = "")
        : ModifiableProperty<T>(name), defaultValue(defaultValue) { }
    virtual ~PropertyMap() { }

    PropertyMap(const PropertyMap<T> &other) = default;
    PropertyMap &operator=(const PropertyMap<T> &rhs) = default;

    PropertyMap(PropertyMap<T> &&other) = default;
    PropertyMap &operator=(PropertyMap<T> &&rhs) = default;

    const T &getDefaultValue() const { return defaultValue; }
    void setDefaultValue(const T &val) {
        auto oldDefault = defaultValue;
        defaultValue = val;
        this->updateObservers(nullptr, oldDefault, defaultValue);
    }

    bool isSetExplicitly(const GraphArtifact *ga) const {
        return map.count(ga) > 0;
    }

    virtual void setValue(const GraphArtifact *ga, const T &value) override {
        if (!this->observable.hasObservers()) {
            map[ga] = value;
        } else {
            auto oldValue = getValue(ga);
            map[ga] = value;
            this->updateObservers(ga, oldValue, value);
        }
    }

    void resetToDefault(const GraphArtifact *ga) {
        auto i = map.find(ga);
        if (i != map.end()) {
            auto oldValue = i->second;
            map.erase(i);
            this->updateObservers(ga, oldValue, defaultValue);
        }
    }

    void resetAll() {
        if (this->observable.hasObservers()) {
            for (const auto &[ga, oldValue] : map) {
                map[ga] = defaultValue;
                this->updateObservers(ga, oldValue, defaultValue);
            }
        }
        map.clear();
    }

    virtual T &operator[](const GraphArtifact *ga) override {
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

    virtual void setAll(const T &val) override {
        map.clear();
        setDefaultValue(val);
    }


private:
    T defaultValue;
    MapType map;
};

}

#endif // PROPERTYMAP_H
