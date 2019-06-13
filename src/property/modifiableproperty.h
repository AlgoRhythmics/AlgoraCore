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

#ifndef MODIFIABLEPROPERTY_H
#define MODIFIABLEPROPERTY_H

#include "property.h"
#include "observable.h"
#include <functional>

namespace Algora {

template<typename T>
class ModifiableProperty : public Property<T>
{
public:
    typedef typename std::function<void(GraphArtifact *ga, const T &oldValue, const T &newValue)> PropertyChangeAction;

    ModifiableProperty(const std::string &name = "")
        : Property<T>(name) { }
    virtual ~ModifiableProperty() { }

    ModifiableProperty(const ModifiableProperty<T> &other)
        : Property<T>(other) {
        // do not copy observable
    }
    ModifiableProperty &operator=(const ModifiableProperty<T> &other) {
        if (this == &other) {
            return *this;
        }
        Property<T>::operator=(other);
        // do not copy observable

        return *this;
    }
    ModifiableProperty(ModifiableProperty<T> &&other) = default;
    ModifiableProperty &operator=(ModifiableProperty<T> &&other) = default;

    virtual void setValue(const GraphArtifact *ga, const T &value) = 0;

    virtual T &operator[](const GraphArtifact *ga) = 0;

    virtual void setAll(const T &val) = 0;

    void onPropertyChange(void *id, const PropertyChangeAction &fun) {
        observable.addObserver(id, fun);
    }

    void removeOnPropertyChange(void *id) {
        observable.removeObserver(id);
    }

protected:
    Observable<GraphArtifact*, T, T> observable;

    void updateObservers(const GraphArtifact *cga, const T &oldValue, const T&newValue) {
        auto *ga = const_cast<GraphArtifact*>(cga);
        observable.notifyObservers(ga, oldValue,newValue);
    }
};

}

#endif // MODIFIABLEPROPERTY
