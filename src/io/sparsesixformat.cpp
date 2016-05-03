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
    int resultStart = result.size();
    splitAndConvertBitset(bpad, 6, result);
    for (unsigned int i = resultStart; i < result.size(); i++) {
        result[i] += 63;
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

void prependBitset(boost::dynamic_bitset<> &bitset, boost::dynamic_bitset<> &p)
{
   bitset.resize(bitset.size() + p.size());
   bitset <<=  p.size();
   p.resize(bitset.size());
   bitset |= p;
}

void prependBitset(boost::dynamic_bitset<> &bitset, bool b)
{
    bitset.resize(bitset.size() + 1);
    bitset <<= 1;
    bitset[0] = b;
}

void reverseBitset(boost::dynamic_bitset<> &bitset)
{
    for (unsigned int i = 0; i < bitset.size() / 2; i++) {
        bool x = bitset[i];
        bitset[i] = bitset[bitset.size() - i];
        bitset[bitset.size() - i] = x;
    }
}

void splitAndConvertBitset(boost::dynamic_bitset<> &bitset, int chunkSize, std::vector<int> &result)
{
    int blocks = bitset.size() / chunkSize;
    boost::dynamic_bitset<> mask(bitset.size(), (1 << (chunkSize + 1) ) - 1); // 63 = 111111
    mask <<= (blocks - 1) * chunkSize;
    for (unsigned int i = blocks; i > 0; i--) {
        //std::cout << "mask: " << mask << std::endl;
        boost::dynamic_bitset<> chunk(bitset.size());
        chunk = (bitset & mask) >> (i-1)*chunkSize;
        chunk.resize(chunkSize);
        int value = chunk.to_ulong();
        result.push_back(value);
        //std::cout << "value: " << value << std::endl;
        mask >>= chunkSize;
    }

}

void asciiToInts(std::istream &in, std::vector<int> &bytes, char breakAt)
{
    char c;
    while ((in.peek() != breakAt) && (in >> c)) {
       int i = c;
       bytes.push_back(i);
    }
}

unsigned long long extractSparseSixN(std::vector<int> &bytes)
{
    if (bytes.size() < 1) {
        return 0;
    }
    //std::cout << "first byte: " << bytes[0] << std::endl;
    if (bytes[0] != 126) {
        int value = bytes[0] - 63;
        //std::cout << "value: " << value << std::endl;
        bytes.erase (bytes.begin());
        return value;
    } else if (bytes.size() < 4) {
        return 0;
    }
    //std::cout << "second byte: " << bytes[1] << std::endl;
    //std::cout << "third byte: " << bytes[2] << std::endl;
    //std::cout << "forth byte: " << bytes[3] << std::endl;
    if (bytes[1] != 126) {
        boost::dynamic_bitset<> b1(6, bytes[1] - 63);
        boost::dynamic_bitset<> b2(6, bytes[2] - 63);
        boost::dynamic_bitset<> b3(6, bytes[3] - 63);
        //std::cout << "bits 1: " << b1 << std::endl;
        //std::cout << "bits 2: " << b2 << std::endl;
        //std::cout << "bits 3: " << b3 << std::endl;
        bytes.erase(bytes.begin(), bytes.begin()+4);
        prependBitset(b1, b2);
        //std::cout << "bits 1+2: " << b1 << std::endl;
        prependBitset(b1, b3);
        //std::cout << "bits: " << b1 << std::endl;
        //std::cout << "ulong: " << b1.to_ulong() << std::endl;
        return b1.to_ulong();
    } else if (bytes.size() < 8) {
        return 0;
    }
    boost::dynamic_bitset<> b1(6, bytes[2] - 63);
    boost::dynamic_bitset<> b2(6, bytes[3] - 63);
    boost::dynamic_bitset<> b3(6, bytes[4] - 63);
    boost::dynamic_bitset<> b4(6, bytes[5] - 63);
    boost::dynamic_bitset<> b5(6, bytes[6] - 63);
    boost::dynamic_bitset<> b6(6, bytes[7] - 63);
    bytes.erase (bytes.begin(), bytes.begin() + 8);
    prependBitset(b1, b2);
    prependBitset(b1, b3);
    prependBitset(b1, b4);
    prependBitset(b1, b5);
    prependBitset(b1, b6);
    return b1.to_ulong();
}

void bytesToBitset(std::vector<int> &bytes, boost::dynamic_bitset<> &bitset)
{
    for (int b : bytes) {
        boost::dynamic_bitset<> bs(6, b - 63);
        prependBitset(bitset, bs);
    }
}

bool extractLeftMostBit(boost::dynamic_bitset<> &bitset)
{
   bool b = bitset[bitset.size() - 1];
   bitset.resize(bitset.size() - 1);
   return b;
}

void extractLeftMostKBits(boost::dynamic_bitset<> &bitset, unsigned int k, boost::dynamic_bitset<> &kbits)
{
    kbits.resize(k);
    for (unsigned int i = 1; i <= k; i++) {
        kbits[k - i] = bitset[bitset.size() - i];
    }
    bitset.resize(bitset.size() - k);
}

}

