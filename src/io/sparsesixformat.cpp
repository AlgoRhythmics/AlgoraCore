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

//#include <iostream>

namespace Algora {

void sparseSixR(boost::dynamic_bitset<> &bits, std::vector<int> &result) {
    unsigned int blocks = bits.size() / 6;
    if (bits.size() > blocks * 6) {
        blocks++;
    }
    //std::cout << "blocks: " << blocks << std::endl;
    boost::dynamic_bitset<> bpad(bits);
    bpad.resize(6 * blocks);
    bpad <<= (6 * blocks - bits.size());
    boost::dynamic_bitset<> mask(bpad.size(), 63); // 63 = 111111
    mask <<= (blocks - 1) * 6;
    //std::cout << "bpad: " << bpad << std::endl;
    for (unsigned int i = blocks; i > 0; i--) {
        //std::cout << "mask: " << mask << std::endl;
        boost::dynamic_bitset<> chunk(bpad.size());
        chunk = (bpad & mask) >> (i-1)*6;
        chunk.resize(6);
        int value = chunk.to_ulong() + 63;
        result.push_back(value);
        //std::cout << "value: " << value << std::endl;
        mask >>= 6;
    }
}

void sparseSixN(unsigned long long n, std::vector<int> &result)
{
   if (n <= 62ull) {
       result.push_back((int) n + 63);
   } else if (n <= 258047ull) {
        result.push_back(126);
        boost::dynamic_bitset<> bits(18,n);
        sparseSixR(bits, result);
   } else if (n <= 68719476735ull) {
        result.push_back(126);
        result.push_back(126);
        boost::dynamic_bitset<> bits(36,n);
        sparseSixR(bits, result);
   }
}

void printAscii(std::ostream &out, std::vector<int> &bytes)
{
    for(int b : bytes) {
       char c = b;
       out << c;
    }
}

}

