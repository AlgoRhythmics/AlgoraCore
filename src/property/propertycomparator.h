// license
#ifndef PROPERTYCOMPARATOR_H
#define PROPERTYCOMPARATOR_H

namespace Algora {

class GraphArtifact;

template<typename PropertyType>
class PropertyComparator
{
public:
    PropertyComparator(PropertyType *p, bool less = true)
        : property(p), useLess(less) {}

    bool operator()(const GraphArtifact *lhs, const GraphArtifact *rhs) const {
        return useLess
                ? (*property)(lhs) <= (*property)(rhs)
                : (*property)(lhs) >= (*property)(rhs);
    }
private:
    PropertyType *property;
    bool useLess;
};

}

#endif // PROPERTYCOMPARATOR_H
