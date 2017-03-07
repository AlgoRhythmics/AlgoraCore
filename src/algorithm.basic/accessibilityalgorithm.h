// license
#ifndef ACCESSIBILITYALGORITHM_H
#define ACCESSIBILITYALGORITHM_H

#include "algorithm/propertycomputingalgorithm.h"

namespace Algora {

class Vertex;

class AccessibilityAlgorithm : public PropertyComputingAlgorithm<void, bool>
{
public:
    AccessibilityAlgorithm(bool computeValues = true);
    virtual ~AccessibilityAlgorithm();

    bool canAccess(Vertex *source, Vertex *target);

    // DiGraphAlgorithm interface
public:
    virtual void run() override;
    virtual std::string getName() const noexcept override { return "Accessibility Algorithm"; }
    virtual std::string getShortName() const noexcept override { return "Accessibility"; }

protected:
    virtual void onDiGraphSet() override;

    // ValueComputingAlgorithm interface
public:
    virtual void deliver() override { }

private:
    struct CheshireCat;
    CheshireCat *grin;
};

}

#endif // ACCESSIBILITYALGORITHM_H
