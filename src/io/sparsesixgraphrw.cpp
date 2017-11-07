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


#include "sparsesixgraphrw.h"

#include "sparsesixformat.h"
#include "graph/digraph.h"
#include "graph/parallelarcsbundle.h"
#include "property/propertymap.h"

#include <ostream>
#include <cmath>
#include <tuple>
#include <algorithm>

//#include <iostream>

namespace Algora {

SparseSixGraphRW::SparseSixGraphRW()
{

}

void SparseSixGraphRW::processGraph(const DiGraph *graph)
{
    if (StreamDiGraphWriter::outputStream == 0) {
        return;
    }

    std::ostream &outputStream = *(StreamDiGraphWriter::outputStream);
    DiGraph *ncGraph = const_cast<DiGraph*>(graph);

    PropertyMap<int> vertexId(-1);
    int i = 0;
    ncGraph->mapVertices([&](Vertex *v) { vertexId[v] = i++; });

    int n = ncGraph->getSize();
    outputStream << ":";
    std::vector<int> bytes;
    sparseSixN(n, bytes);
    printAscii(outputStream, bytes);
    bytes.clear();

    std::vector<std::tuple<int,int,bool> > arcs;
    ArcMapping createTuple = [&](Arc *a) {
        int h = vertexId[a->getHead()];
        int t = vertexId[a->getTail()];
        if (h <= t) {
            arcs.push_back(std::make_tuple(t, h, true));
        } else {
            arcs.push_back(std::make_tuple(h, t, false));
        }
    };

    ncGraph->mapArcs([&](Arc *arc) {
        ParallelArcsBundle *pa = dynamic_cast<ParallelArcsBundle*>(arc);
        if (!pa) {
            createTuple(arc);
        } else {
            pa->mapArcs(createTuple);
        }
    });
    std::sort(arcs.begin(), arcs.end());

    int k = 1;
    while ((1 << k) < n) k++; //ceil(log2(n));
    //std::cout << "k: " << k << std::endl;
    boost::dynamic_bitset<> edgeBits;
    boost::dynamic_bitset<> directionBits;

    auto extendEdgeBits = [&](int w) {
        edgeBits.resize(edgeBits.size() + k);
        edgeBits <<= k;
        boost::dynamic_bitset<> enc(edgeBits.size(), w);
        edgeBits |= enc;
    };

    int cur = 0;
    int v, u;
    bool direction;
    for (auto t : arcs) {
        std::tie(v, u, direction) = t;
        //std::cout << "Processing (" << v << "," << u << "," << direction << ")" << std::endl;
        if (v == cur) {
            prependBitset(edgeBits, false);
            extendEdgeBits(u);
        } else if (v == cur + 1) {
            cur++;
            prependBitset(edgeBits, true);
            extendEdgeBits(u);
        } else {
            cur = v;
            prependBitset(edgeBits, true);
            extendEdgeBits(v);
            prependBitset(edgeBits, false);
            extendEdgeBits(u);
        }
        //std::cout << edgeBits << std::endl;
        directionBits.push_back(direction);
    }
    int pad = 6 - (edgeBits.size() % 6);
    if (pad < 6) {
        edgeBits.resize(edgeBits.size() + pad);
        edgeBits <<= pad;
        if (k < 6 && n == (1 << k) && pad > k && cur < n-1) {
            edgeBits[pad - 1] = false;
            for (int i = 0; i < pad - 1; i++) {
                edgeBits[i] = true;
            }
        } else {
            for (int i = 0; i < pad; i++) {
                edgeBits[i] = true;
            }
        }
    }
    // reverse direction bits
    //std::cout << "direction: " << directionBits << std::endl;
    reverseBitset(directionBits);
    //std::cout << "direction: " << directionBits << std::endl;

    //std::cout << "edge bits: " << edgeBits << std::endl;
    sparseSixR(edgeBits, bytes);
    printAscii(outputStream, bytes);
    outputStream << ":";
    bytes.clear();
    sparseSixR(directionBits, bytes);
    printAscii(outputStream, bytes);
    outputStream << std::endl;
}

bool SparseSixGraphRW::provideDiGraph(DiGraph *graph)
{
    if (StreamDiGraphReader::inputStream == 0) {
        return false;
    }
    std::istream &inputStream = *(StreamDiGraphReader::inputStream);
    char colon;
    inputStream >> colon;
    if (colon != ':') {
        return false;
    }
    std::vector<int> bytes;
    std::vector<int> direction;
    asciiToInts(inputStream, bytes, ':');
    inputStream >> colon;
    if (colon != ':') {
        return false;
    }
    asciiToInts(inputStream, direction, '\n');
    int n = extractSparseSixN(bytes);
    //std::cout << "n = " << n << std::endl;
    unsigned int k = 1;
    while ((1 << k) < n) k++;
    //std::cout << "k = " << k << std::endl;

    std::vector<Vertex*> vertices;
    for (int i = 0; i < n; i++) {
        vertices.push_back(graph->addVertex());
    }
    boost::dynamic_bitset<> edgeBits;
    boost::dynamic_bitset<> directionBits;
    bytesToBitset(bytes, edgeBits);
    bytesToBitset(direction, directionBits);

    int cur = 0;
    bool b;
    boost::dynamic_bitset<> vBits;
    int v;
    while (edgeBits.size() > k) {
        b = extractLeftMostBit(edgeBits);
        extractLeftMostKBits(edgeBits, k, vBits);
        v = vBits.to_ulong();
        //std::cout << b << " " << v << std::endl;
        if (b) {
            cur++;
        }
        if (v >= n || cur >= n) {
            break;
        } else if (v > cur) {
            cur = v;
        } else {
            bool d = extractLeftMostBit(directionBits);
            if (d) {
                graph->addArc(vertices.at(cur), vertices.at(v));
                //std::cout << "(" << cur << "," << v << ")" << std::endl;
            } else {
                graph->addArc(vertices.at(v), vertices.at(cur));
                //std::cout << "(" << v << "," << cur << ")" << std::endl;
            }
        }
    }

    return true;
}

}
