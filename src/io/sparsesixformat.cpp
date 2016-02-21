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


#include "sparsesixformat.h"

namespace Algora {

void sparseSixR(boost::dynamic_bitset<> &bits, std::vector<int> &result) {
    unsigned int blocks = bits.size() / 6;
    if (bits.size() > blocks * 6) {
        blocks++;
    }
    boost::dynamic_bitset<> bpad(6 * blocks);
    bpad = bits;
    boost::dynamic_bitset<> mask(bpad.size(), 63); // 63 = 111111
    for (unsigned int i = 0; i < blocks; i++) {
        mask <<= i*6;
        boost::dynamic_bitset<> chunk(bpad.size());
        chunk = (bpad & mask) >> i*6;
        chunk.resize(6);
        int value = chunk.to_ulong() + 63;
        result.push_back(value);
    }
}

}

