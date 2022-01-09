#ifndef MPFRHELPER_H
#define MPFRHELPER_H

#include <boost/multiprecision/mpfr.hpp>

namespace MathOps
{

template<typename T>
T MathOpFunctionGetConstantPi() { return boost::math::constants::pi<T>(); }

template<typename T>
T MathOpFunctionGetConstantE() { return boost::math::constants::e<T>(); }

boost::multiprecision::mpfr_float MathOpFunctionModf(boost::multiprecision::mpfr_float x, boost::multiprecision::mpfr_float& integral);
boost::multiprecision::mpfr_float MathOpFunctionIsnan(boost::multiprecision::mpfr_float x);
boost::multiprecision::mpfr_float MathOpFunctionAbs(boost::multiprecision::mpfr_float x);

boost::multiprecision::mpfr_float MathOpFunctionLog(boost::multiprecision::mpfr_float x);
boost::multiprecision::mpfr_float MathOpFunctionSqrt(boost::multiprecision::mpfr_float x);
boost::multiprecision::mpfr_float MathOpFunctionSin(boost::multiprecision::mpfr_float x);
boost::multiprecision::mpfr_float MathOpFunctionAsin(boost::multiprecision::mpfr_float x);
boost::multiprecision::mpfr_float MathOpFunctionCos(boost::multiprecision::mpfr_float x);
boost::multiprecision::mpfr_float MathOpFunctionAcos(boost::multiprecision::mpfr_float x);
boost::multiprecision::mpfr_float MathOpFunctionTan(boost::multiprecision::mpfr_float x);
boost::multiprecision::mpfr_float MathOpFunctionAtan(boost::multiprecision::mpfr_float x);
boost::multiprecision::mpfr_float MathOpFunctionPow(boost::multiprecision::mpfr_float a, boost::multiprecision::mpfr_float b);

} /* namespace MathOps */

#endif /* MPFRHELPER_H */
