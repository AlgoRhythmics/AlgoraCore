#ifndef REVERSEARC_H
#define REVERSEARC_H

#include "arc.h"

namespace Algora {

class ReverseArc : public Arc
{
public:
    ReverseArc(Arc *a) : Arc(a->getHead(), a->getTail(), a->getParent()), arc(a) { }
    virtual ~ReverseArc() {}

    // GraphArtifact interface
public:
    virtual std::string typeString() const noexcept override { return arc->typeString() + " [reversed]"; }
    virtual std::string toString() const override { return arc->toString() + "^R"; }

    // Arc interface
public:
    virtual unsigned int getSize() const override { return arc->getSize(); }

private:
    Arc *arc;
};

}

#endif // REVERSEARC_H
