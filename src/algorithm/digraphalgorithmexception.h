// license
#ifndef ALGORITHMEXCEPTION_H
#define ALGORITHMEXCEPTION_H

#include <stdexcept>
#include "digraphalgorithm.h"

#include <iostream>
#include <sstream>

namespace Algora {


class DiGraphAlgorithmException : public std::logic_error
{
public:
    explicit DiGraphAlgorithmException(DiGraphAlgorithm *a, const std::string &what_arg)
        : std::logic_error(a->getName() + " : " + what_arg) { }
    explicit DiGraphAlgorithmException(DiGraphAlgorithm *a, const char *what_arg)
        : std::logic_error(a->getName() + " : " + what_arg) { }
    explicit DiGraphAlgorithmException(const std::string &what_arg)
        : std::logic_error("Anonymous algorithm : " + what_arg) { }
    explicit DiGraphAlgorithmException(const char *what_arg)
        : std::logic_error(std::string("Anonymous algorithm : ") + what_arg) { }
};

}

#endif // ALGORITHMEXCEPTION_H
