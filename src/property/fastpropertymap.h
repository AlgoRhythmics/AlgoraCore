/**
 * Copyright (C) 2013 - 2018 : Kathrin Hanauer
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

#ifndef FASTPROPERTYMAP_H
#define FASTPROPERTYMAP_H

#include "modifiableproperty.h"
#include "graph/graphartifact.h"

#include <vector>

namespace Algora {

template<typename T>
class FastPropertyMap : public ModifiableProperty<T>
{
public:
    typedef typename std::vector<T>::iterator iterator;
    typedef typename std::vector<T>::const_iterator const_iterator;
    typedef typename std::vector<T>::reference reference;

    FastPropertyMap(unsigned int capacity = 0, const T &defaultValue = T(), const std::string &name = "")
        : ModifiableProperty<T>(name), defaultValue(defaultValue) { buckets.assign(capacity, defaultValue); }
    FastPropertyMap(const FastPropertyMap<T> &other)
        : ModifiableProperty<T>(other),
          defaultValue(other.defaultValue),
          buckets(other.buckets) { }
    virtual ~FastPropertyMap() { }

    FastPropertyMap &operator=(const FastPropertyMap<T> &rhs) {
        if (this == &rhs) {
            return *this;
        }
        ModifiableProperty<T>::operator=(rhs);
        defaultValue = rhs.defaultValue;
        buckets = rhs.buckets;
        return *this;
    }

    const T &getDefaultValue() const { return defaultValue; }

    void setDefaultValue(const T &val) { defaultValue = val; buckets.assign(buckets.size(), defaultValue); }

    virtual void setValue(const GraphArtifact *ga, const T &value) override {
        unsigned int id = ga->getId();
        enlarge(id);
        buckets[id] = value;
    }

    void resetToDefault(const GraphArtifact *ga) {
        setValue(ga, defaultValue);
    }

    void resetAll(unsigned int capacity) {
        buckets.assign(capacity, defaultValue);
    }

    void resetAll() {
        resetAll(buckets.size());
    }

    virtual T &operator[](const GraphArtifact *ga) override {
        unsigned int id = ga->getId();
        enlarge(id);
        return buckets[id];
    }

    // iterators
    iterator begin() {
        return buckets.begin();
    }

    const_iterator begin() const {
        return buckets.begin();
    }

    iterator end() {
        return buckets.end();
    }

    const_iterator end() const {
        return buckets.end();
    }

    const_iterator cbegin() const noexcept {
        return buckets.cbegin();
    }
    const_iterator cend() const noexcept {
        return buckets.cend();
    }

    // Property interface
public:
    virtual T getValue(const GraphArtifact *ga) const override {
        unsigned int id = ga->getId();
        if (id < buckets.size()) {
            return buckets.at(id);
        }
        return defaultValue;
    }

    virtual void setAll(const T &val) override {
      setDefaultValue(val);
    }

private:
    void enlarge(unsigned int size) {
        if (size < buckets.size()) {
            return;
        }
        buckets.resize(size + 1, defaultValue);
    }

    T defaultValue;
    typename std::vector<T> buckets;
};

template<>
class FastPropertyMap<bool> : public ModifiableProperty<bool>
{
public:
    typedef typename std::vector<char>::reference reference;

    FastPropertyMap(unsigned int capacity = 0, const bool &defaultValue = bool(), const std::string &name = "")
        : ModifiableProperty<bool>(name), defaultValue(defaultValue) { buckets.assign(capacity, defaultValue); }
    FastPropertyMap(const FastPropertyMap<bool> &other)
        : ModifiableProperty<bool>(other),
          defaultValue(other.defaultValue),
          buckets(other.buckets) { }
    virtual ~FastPropertyMap() { }

    FastPropertyMap &operator=(const FastPropertyMap<bool> &rhs) {
        if (this == &rhs) {
            return *this;
        }
        ModifiableProperty<bool>::operator=(rhs);
        defaultValue = rhs.defaultValue;
        buckets = rhs.buckets;
        return *this;
    }

    const bool &getDefaultValue() const { return defaultValue; }
    void setDefaultValue(const bool &val) { defaultValue = val; buckets.assign(buckets.size(), defaultValue); }

    virtual void setValue(const GraphArtifact *ga, const bool &value) override {
        unsigned int id = ga->getId();
        enlarge(id);
        buckets[id] = value;
    }

    void resetToDefault(const GraphArtifact *ga) {
        setValue(ga, defaultValue);
    }

    void resetAll(unsigned int capacity = 0U) {
        if (capacity == 0) {
            capacity = buckets.size();
        }
        buckets.assign(capacity, defaultValue);
    }

    virtual bool &operator[](const GraphArtifact *ga) override {
        unsigned int id = ga->getId();
        enlarge(id);
        //bool &b = (bool&) buckets[id];
        //return b;
        return (bool&) buckets[id];
    }

    // Property interface
public:
    virtual bool getValue(const GraphArtifact *ga) const override {
        unsigned int id = ga->getId();
        if (id < buckets.size()) {
            return buckets.at(id);
        }
        return defaultValue;
    }

    virtual void setAll(const bool &val) override {
      setDefaultValue(val);
    }

private:
    void enlarge(unsigned int size) {
        if (size < buckets.size()) {
            return;
        }
        buckets.resize(size + 1, defaultValue);
    }

    bool defaultValue;
    typename std::vector<char> buckets;
};

}

#endif // FASTPROPERTYMAP_H
