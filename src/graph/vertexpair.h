// license
#ifndef VERTEXPAIR_H
#define VERTEXPAIR_H

#include "graphartifact.h"

namespace Algora {

class Vertex;

class VertexPair : public GraphArtifact
{
public:
    explicit VertexPair(Vertex *first, Vertex *second, GraphArtifact *parent)
        : GraphArtifact(parent), first(first), second(second) {}

    virtual ~VertexPair() { }

    Vertex *getFirst() const { return first; }
    Vertex *getSecond() const { return second; }

    // GraphArtifact interface
public:
    virtual std::string typeString() const override {
        return "VertexPair";
    }
    virtual std::string toString() const override;

protected:
    Vertex *first;
    Vertex *second;
};

inline bool operator==(const VertexPair &lhs, const VertexPair &rhs){
    return (lhs.getFirst() == rhs.getFirst()) &&  (lhs.getSecond() == rhs.getSecond());
}
inline bool operator< (const VertexPair &lhs, const VertexPair &rhs){
    return lhs.getFirst() < rhs.getFirst()
            || (lhs.getFirst() == rhs.getFirst() && lhs.getSecond() < rhs.getSecond());
}
inline bool operator!=(const VertexPair &lhs, const VertexPair &rhs){ return !operator==(lhs,rhs);}
inline bool operator> (const VertexPair &lhs, const VertexPair &rhs){ return  operator< (rhs,lhs);}
inline bool operator<=(const VertexPair &lhs, const VertexPair &rhs){ return !operator> (lhs,rhs);}
inline bool operator>=(const VertexPair &lhs, const VertexPair &rhs){ return !operator< (lhs,rhs);}

}

#endif // VERTEXPAIR_H
