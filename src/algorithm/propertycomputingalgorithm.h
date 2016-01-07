/**
 * Copyright (C) 2013 - 2015 : Kathrin Hanauer
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


#ifndef PROPERTYCOMPUTINGALGORITHM_H
#define PROPERTYCOMPUTINGALGORITHM_H

#include "valuecomputingalgorithm.h"

namespace Algora {

template<typename T>
class PropertyMap;

template<typename ValueType, typename PropertyType>
class PropertyComputingAlgorithm : public ValueComputingAlgorithm<ValueType>
{
public:
    PropertyComputingAlgorithm() : propertyMap(0) { }
    virtual ~PropertyComputingAlgorithm() { }

    virtual void usePropertyMap(PropertyMap<PropertyType> *map) {
        this->propertyMap = map;
    }

    // DiGraphAlgorithm interface
public:
    virtual bool prepare() override {
        return ValueComputingAlgorithm<ValueType>::prepare() && propertyMap != 0;
    }

protected:
    PropertyMap<PropertyType> *propertyMap;
};

}

#endif // PROPERTYCOMPUTINGALGORITHM_H
