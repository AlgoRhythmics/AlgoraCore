// license
#ifndef BICONNECTEDCOMPONENTSALGORITHM_H
#define BICONNECTEDCOMPONENTSALGORITHM_H

#include "algorithm/propertycomputingalgorithm.h"
#include <vector>

namespace Algora {

class BiconnectedComponentsAlgorithm : public PropertyComputingAlgorithm<int, std::vector<int> >
{
public:
    BiconnectedComponentsAlgorithm();
    virtual ~BiconnectedComponentsAlgorithm();

    // DiGraphAlgorithm interface
public:
    virtual void run() override;
    virtual std::string getName() const noexcept override { return "BiconnectedComponents"; }
    virtual std::string getShortName() const noexcept override { return "bic"; }

    // ValueComputingAlgorithm interface
public:
    virtual int deliver() override;

private:
    int numBics;
};

}

#endif // BICONNECTEDCOMPONENTSALGORITHM_H
