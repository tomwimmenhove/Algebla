#ifndef CONSTANTS_H
#define CONSTANTS_H

#include "algeblah.h"

namespace MathOps
{

/* Forward declarations */
template<typename T> struct MathOp;
template<typename T> struct Container;
template<typename T> struct ConstantSymbol;
template<typename T> struct Variable;
template<typename T> struct ValueVariable;
template<typename T> struct NamedConstant;
template<typename T> struct MutableValue;
template<typename T> struct ConstantValue;

struct Constants
{
    template <typename T>
    static std::shared_ptr<MathOp<T>> pi() { return ConstantSymbol<T>::create("%pi", get_constant_pi<T>()); }

    template <typename T>
    static std::shared_ptr<MathOp<T>> e() { return ConstantSymbol<T>::create("%e", get_constant_e<T>()); }

private:
    Constants() = delete;
};

} /* MathOps */

#endif /* CONSTANTS_H */