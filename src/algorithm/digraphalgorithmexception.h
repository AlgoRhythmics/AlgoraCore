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

#ifndef ALGORITHMEXCEPTION_H
#define ALGORITHMEXCEPTION_H

#include <stdexcept>
#include "digraphalgorithm.h"

#include <iostream>
#include <sstream>

namespace Algora {


class DiGraphAlgorithmException : public std::logic_error
{
public:
    explicit DiGraphAlgorithmException(DiGraphAlgorithm *a, const std::string &what_arg)
        : std::logic_error(a->getName() + " : " + what_arg) { }
    explicit DiGraphAlgorithmException(DiGraphAlgorithm *a, const char *what_arg)
        : std::logic_error(a->getName() + " : " + what_arg) { }
    explicit DiGraphAlgorithmException(const std::string &what_arg)
        : std::logic_error("Anonymous algorithm : " + what_arg) { }
    explicit DiGraphAlgorithmException(const char *what_arg)
        : std::logic_error(std::string("Anonymous algorithm : ") + what_arg) { }
};

}

#endif // ALGORITHMEXCEPTION_H
