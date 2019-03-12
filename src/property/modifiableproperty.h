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


namespace Algora {

template<typename T>
class ModifiableProperty : public Property<T>
{
public:
    ModifiableProperty(const std::string &name = "")
        : Property<T>(name) { }
    ModifiableProperty(const ModifiableProperty<T> &other)
        : Property<T>(other) { }
    virtual ~ModifiableProperty() { }

    virtual void setValue(const GraphArtifact *ga, const T &value) = 0;

    virtual T &operator[](const GraphArtifact *ga) = 0;

    virtual void setAll(const T &val) = 0;
};

}

#endif // MODIFIABLEPROPERTY
