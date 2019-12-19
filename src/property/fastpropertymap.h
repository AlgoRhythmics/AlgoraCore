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

#ifndef FASTPROPERTYMAP_H
#define FASTPROPERTYMAP_H

#include "modifiableproperty.h"
#include "graph/graphartifact.h"
#include <cassert>

#include <vector>

namespace Algora {

template<typename T>
class FastPropertyMap : public ModifiableProperty<T>
{
public:
    typedef typename std::vector<T>::iterator iterator;
    typedef typename std::vector<T>::const_iterator const_iterator;
    typedef typename std::vector<T>::reference reference;
    typedef typename std::vector<T>::size_type size_type;

    FastPropertyMap(const T &defaultValue = T(), const std::string &name = "",
                    size_type capacity = 0)
        : ModifiableProperty<T>(name), defaultValue(defaultValue) {
        buckets.assign(capacity, defaultValue);
    }
    virtual ~FastPropertyMap() { }


    FastPropertyMap(const FastPropertyMap<T> &other) = default;
    FastPropertyMap &operator=(const FastPropertyMap<T> &rhs) = default;
    FastPropertyMap(FastPropertyMap<T> &&other) = default;
    FastPropertyMap &operator=(FastPropertyMap<T> &&rhs) = default;

    const T &getDefaultValue() const { return defaultValue; }

    void setDefaultValue(const T &val) {
        defaultValue = val;
    }

    void setValueAtId(GraphArtifact::id_type id, const T &value) {
        enlarge(id);
        buckets[id] = value;
    }

    virtual void setValue(const GraphArtifact *ga, const T &value) override {
        auto id = ga->getId();
        auto oldValue = getValueAtId(id);
        setValueAtId(id, value);
        this->updateObservers(ga, oldValue, value);
    }

    void resetAtId(GraphArtifact::id_type id) {
        setValueAtId(id, defaultValue);
    }

    void resetToDefault(const GraphArtifact *ga) {
        setValue(ga, defaultValue);
    }

    bool hasDefaultValue(const GraphArtifact *ga) {
        return defaultValue == getValue(ga);
    }

    void fit() {
        buckets.shrink_to_fit();
    }

    void resetAll(size_type capacity) {
        buckets.assign(capacity, defaultValue);
        assert(buckets.size() == capacity);
        if (capacity == 0) {
            fit();
        }
        this->updateObservers(nullptr, defaultValue, defaultValue);
    }

    void resetAll() {
        resetAll(buckets.size());
    }

    size_type size() const {
        return buckets.size();
    }

    virtual T &operator[](const GraphArtifact *ga) override {
        auto id = ga->getId();
        enlarge(id);
        return buckets[id];
    }

    T &operator[](GraphArtifact::id_type id) {
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

    T getValueAtId(GraphArtifact::id_type id) const {
        if (id < buckets.size()) {
            return buckets.at(id);
        }
        return defaultValue;
    }

    // Property interface
public:
    virtual T getValue(const GraphArtifact *ga) const override {
        auto id = ga->getId();
        return getValueAtId(id);
    }

    virtual void setAll(const T &val) override {
      setDefaultValue(val);
    }

private:
    void enlarge(size_type size) {
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
    typedef typename std::vector<char>::size_type size_type;

    FastPropertyMap(const bool &defaultValue = false, const std::string &name = "", size_type capacity = 0)
        : ModifiableProperty<bool>(name), defaultValue(defaultValue) { buckets.assign(capacity, defaultValue); }
    virtual ~FastPropertyMap() override { }


    FastPropertyMap(const FastPropertyMap<bool> &other) = default;
    FastPropertyMap &operator=(const FastPropertyMap<bool> &rhs) = default;
    FastPropertyMap(FastPropertyMap<bool> &&other) = default;
    FastPropertyMap &operator=(FastPropertyMap<bool> &&rhs) = default;

    const bool &getDefaultValue() const { return defaultValue; }
    void setDefaultValue(const bool &val);

    void setValueAtId(GraphArtifact::id_type id, const bool &value) {
        enlarge(id);
        buckets[id] = value;
    }

    virtual void setValue(const GraphArtifact *ga, const bool &value) override {
        auto id = ga->getId();
        if (this->observable.hasObservers()) {
            auto oldValue = getValueAtId(id);
            setValueAtId(id, value);
            this->updateObservers(ga, oldValue, value);
        } else {
            setValueAtId(id, value);
        }
    }

    void resetToDefault(const GraphArtifact *ga) {
        setValue(ga, defaultValue);
    }

    bool hasDefaultValue(const GraphArtifact *ga) {
        return defaultValue == getValue(ga);
    }

    void fit() {
        buckets.shrink_to_fit();
    }

    void resetAtId(GraphArtifact::id_type id) {
        setValueAtId(id, defaultValue);
    }

    void resetAll(size_type capacity = 0ULL);

    size_type size() const {
        return buckets.size();
    }

    virtual bool &operator[](const GraphArtifact *ga) override {
        auto id = ga->getId();
        enlarge(id);
        return reinterpret_cast<bool&>(buckets[id]);
    }

    bool &operator[](GraphArtifact::id_type id) {
        enlarge(id);
        return reinterpret_cast<bool&>(buckets[id]);
    }

    bool getValueAtId(GraphArtifact::id_type id) const {
        if (id < buckets.size()) {
            return buckets.at(id);
        }
        return defaultValue;
    }

    // Property interface
public:
    virtual bool getValue(const GraphArtifact *ga) const override {
        auto id = ga->getId();
        return getValueAtId(id);
    }

    virtual void setAll(const bool &val) override {
      setDefaultValue(val);
    }

private:
    void enlarge(size_type size) {
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
