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

#ifndef SPARSESIXFORMAT_H
#define SPARSESIXFORMAT_H

#include <vector>
#include <boost/dynamic_bitset.hpp>

namespace Algora {

void sparseSixR(boost::dynamic_bitset<> &bits, std::vector<int> &result);

void sparseSixN(unsigned long long n, std::vector<int> &result);

void printAscii(std::ostream &out, std::vector<int> &bytes);

void prependBitset(boost::dynamic_bitset<> &bitset, boost::dynamic_bitset<> &p);

void prependBitset(boost::dynamic_bitset<> &bitset, bool b);

void reverseBitset(boost::dynamic_bitset<> &bitset);

void splitAndConvertBitset(boost::dynamic_bitset<> &bitset, int chunkSize, std::vector<int> &result);

void asciiToInts(std::istream &in, std::vector<int> &bytes, char breakAt);

unsigned long long extractSparseSixN(std::vector<int> &bytes);

void bytesToBitset(std::vector<int> &bytes, boost::dynamic_bitset<> &bitset);

bool extractLeftMostBit(boost::dynamic_bitset<> &bitset);

void extractLeftMostKBits(boost::dynamic_bitset<> &bitset, unsigned int k, boost::dynamic_bitset<> &kbits);

}

#endif // SPARSESIXFORMAT_H
