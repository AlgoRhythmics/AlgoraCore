// license
#ifndef BICONNECTEDCOMPONENTSALGORITHM_H
#define BICONNECTEDCOMPONENTSALGORITHM_H

#include "algorithm/propertycomputingalgorithm.h"

namespace Algora {

class BiconnectedComponentsAlgorithm : public PropertyComputingAlgorithm<int, int>
{
public:
    BiconnectedComponentsAlgorithm();
    virtual ~BiconnectedComponentsAlgorithm();

    // DiGraphAlgorithm interface
public:
    virtual void run() override;

    // ValueComputingAlgorithm interface
public:
    virtual int deliver() override;

private:
    int numBics;
};

}

#endif // BICONNECTEDCOMPONENTSALGORITHM_H
