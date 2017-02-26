// license
#ifndef LINEARVERTEXSEQUENCETIKZWRITER_H
#define LINEARVERTEXSEQUENCETIKZWRITER_H

#include "streamdigraphwriter.h"

namespace Algora {

class LinearVertexSequenceTikzWriter : public StreamDiGraphWriter
{
public:
    LinearVertexSequenceTikzWriter(std::ostream *output = 0);
    virtual ~LinearVertexSequenceTikzWriter();

    // DiGraphProcessor interface
public:
    virtual void processGraph(const DiGraph *graph, const DiGraphInfo *info) override;

private:
    class CheshireCat;
    CheshireCat *grin;
};

}

#endif // LINEARVERTEXSEQUENCETIKZWRITER_H
