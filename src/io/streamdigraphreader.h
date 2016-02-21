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


#ifndef STREAMDIGRAPHREADER_H
#define STREAMDIGRAPHREADER_H

#include "pipe/digraphprovider.h"
#include <istream>

namespace Algora {

class StreamDiGraphReader : public DiGraphProvider
{
public:
    explicit StreamDiGraphReader(std::istream *input = 0) : inputStream(input) { }
    virtual ~StreamDiGraphReader() { }

    void setInputStream(std::istream *input) { inputStream = input; }

    // DiGraphProvider interface
public:
    virtual bool isGraphAvailable() override {
        return inputStream != 0 && inputStream->good() && inputStream->rdbuf()->in_avail() > 0;
    }

protected:
    std::istream *inputStream;

};

}

#endif // STREAMDIGRAPHREADER_H
