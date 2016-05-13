// license
#ifndef ALGORITHMEXCEPTION_H
#define ALGORITHMEXCEPTION_H

#include <stdexcept>
#include "digraphalgorithm.h"

namespace Algora {


class DiGraphAlgorithmException : public std::logic_error
{
public:
    explicit DiGraphAlgorithmException(DiGraphAlgorithm *a, const std::string &what_arg)
        : std::logic_error(what_arg), algorithm(a) { }
    explicit DiGraphAlgorithmException(DiGraphAlgorithm *a, const char *what_arg)
        : std::logic_error(what_arg), algorithm(a) { }

    virtual const char* what() const noexcept {
        std::string w(algorithm->getName());
        w.append(" : ");
        w.append(std::logic_error::what());
        return w.c_str();
    }

private:
    DiGraphAlgorithm *algorithm;
};

}

#endif // ALGORITHMEXCEPTION_H
