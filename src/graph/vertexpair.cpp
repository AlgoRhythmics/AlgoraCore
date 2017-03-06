// license
#include "vertexpair.h"
#include "vertex.h"

#include <sstream>

namespace Algora {

std::string VertexPair::toString() const
{
    std::ostringstream strStream;
    strStream << typeString()
              << " ("
              << first->toString()
              << " -> "
              << second->toString()
              << ") ["
              << idString() << "]";

    return strStream.str();
}

}
