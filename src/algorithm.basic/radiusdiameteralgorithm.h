#ifndef RADIUSDIAMETERALGORITHM_H
#define RADIUSDIAMETERALGORITHM_H

#include "algorithm/valuecomputingalgorithm.h"

namespace Algora {

class RadiusDiameterAlgorithm : public ValueComputingAlgorithm<int>
{
public:
    static const int INFINITE;

    explicit RadiusDiameterAlgorithm();
    virtual ~RadiusDiameterAlgorithm() {}

    void computeRadius(bool rad) {
        radOrDiam = rad;
    }

    bool computesRadius() const {
        return radOrDiam;
    }

    // DiGraphAlgorithm interface
public:
    virtual void run() override;
    virtual std::string getName() const noexcept override { return "Radius & Diameter Algorithm"; }
    virtual std::string getShortName() const noexcept override { return "rad-diam"; }

protected:
    virtual void onDiGraphSet() override;

    // ValueComputingAlgorithm interface
public:
    virtual int deliver() override { return radOrDiam ? radius : diameter; }

private:
    int radius;
    int diameter;
    bool radOrDiam;
};

}

#endif // RADIUSDIAMETERALGORITHM_H
