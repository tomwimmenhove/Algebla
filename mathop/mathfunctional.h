#ifndef MATHFUNCTIONAL_H
#define MATHFUNCTIONAL_H

#include <functional>

namespace MathOps
{

/* Unary operation helpers */
template <typename T>
struct negate : public std::unary_function<T, T>
{
    T operator()(T x) const
    {
        return -x;
    }
};

template <typename T>
struct logarithm : public std::unary_function<T, T>
{
    T operator()(T x) const
    {
        return log(x);
    }
};

template <typename T>
struct square_root : public std::unary_function<T, T>
{
    T operator()(T x) const
    {
        return sqrt(x);
    }
};

template <typename T>
struct squares : public std::unary_function<T, T>
{
    T operator()(T x) const
    {
        return x * x;
    }
};

template <typename T>
struct sine : public std::unary_function<T, T>
{
    T operator()(T x) const
    {
        return sin(x);
    }
};

template <typename T>
struct inverse_sine : public std::unary_function<T, T>
{
    T operator()(T x) const
    {
        return asin(x);
    }
};

template <typename T>
struct cosine : public std::unary_function<T, T>
{
    T operator()(T x) const
    {
        return cos(x);
    }
};

template <typename T>
struct inverse_cosine : public std::unary_function<T, T>
{
    T operator()(T x) const
    {
        return acos(x);
    }
};

template <typename T>
struct tangent : public std::unary_function<T, T>
{
    T operator()(T x) const
    {
        return tan(x);
    }
};

template <typename T>
struct inverse_tangent : public std::unary_function<T, T>
{
    T operator()(T x) const
    {
        return atan(x);
    }
};

/* Binary operation helpers */
template <typename T>
struct raises : public std::binary_function<T, T, T>
{
    T operator()(T x, T y) const
    {
        return pow(x, y);
    }
};

template <typename T>
struct multiplies : public std::binary_function<T, T, T>
{
    T operator()(T x, T y) const
    {
        return x * y;
    }
};

template <typename T>
struct divides : public std::binary_function<T, T, T>
{
    T operator()(T x, T y) const
    {
        return x / y;
    }
};

template <typename T>
struct plus : public std::binary_function<T, T, T>
{
    T operator()(T x, T y) const
    {
        return x + y;
    }
};

template <typename T>
struct minus : public std::binary_function<T, T, T>
{
    T operator()(T x, T y) const
    {
        return x - y;
    }
};

} /* MathOps */

#endif /* MATHFUNCTIONAL_H */