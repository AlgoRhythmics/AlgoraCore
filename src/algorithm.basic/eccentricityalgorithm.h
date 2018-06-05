#ifndef ECCENTRICITYALGORITHM_H
#define ECCENTRICITYALGORITHM_H

#include "algorithm/valuecomputingalgorithm.h"

namespace Algora {

class Vertex;

class EccentricityAlgorithm : public ValueComputingAlgorithm<int>
{
public:
    static const int INFINITE;

    explicit EccentricityAlgorithm();
    virtual ~EccentricityAlgorithm() {}

    void setVertex(const Vertex *v) { vertex = v; eccentricity = INFINITE; }

    // DiGraphAlgorithm interface
public:
    virtual bool prepare() override;
    virtual void run() override;
    virtual std::string getName() const noexcept override { return "Eccentricity Algorithm"; }
    virtual std::string getShortName() const noexcept override { return "Eccentricity"; }

protected:
    virtual void onDiGraphSet() override;

    // ValueComputingAlgorithm interface
public:
    virtual int deliver() override { return eccentricity; }

private:
    const Vertex *vertex;
    int eccentricity;
};

}


#endif // ECCENTRICITYALGORITHM_H
