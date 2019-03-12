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

#ifndef FUNCTIONPROPERTY_H
#define FUNCTIONPROPERTY_H

#include "property.h"
#include <functional>

namespace Algora {

template<typename T>
class FunctionProperty : public Property<T>
{
public:
    typedef std::function<T(const GraphArtifact *ga)> PropertyFunction;

    explicit FunctionProperty(PropertyFunction fun, const std::string &name = "")
        : Property<T>(name), fun(fun) { }
    FunctionProperty(const FunctionProperty<T> &other)
        : Property<T>(other), fun(other.fun) { }
    virtual ~FunctionProperty() { }

    FunctionProperty &operator=(const FunctionProperty &rhs) {
        if (this == &rhs) {
            return *this;
        }
        Property<T>::operator =(rhs);
        fun = rhs.fun;
        return *this;
    }

    // Property interface
public:
    virtual T getValue(const GraphArtifact *ga) const override { return fun(ga); }

private:
    PropertyFunction fun;
};

}

#endif // FUNCTIONPROPERTY_H
