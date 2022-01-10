#ifndef MPFRHELPER_H
#define MPFRHELPER_H

#include <boost/multiprecision/mpfr.hpp>

namespace MathOps
{

template<typename T>
T HelperFunctionGetConstantPi() { return boost::math::constants::pi<T>(); }

template<typename T>
T HelperFunctionGetConstantE() { return boost::math::constants::e<T>(); }

boost::multiprecision::mpfr_float HelperFunctionModf(boost::multiprecision::mpfr_float x, boost::multiprecision::mpfr_float& integral);
boost::multiprecision::mpfr_float HelperFunctionIsnan(boost::multiprecision::mpfr_float x);
boost::multiprecision::mpfr_float HelperFunctionAbs(boost::multiprecision::mpfr_float x);

boost::multiprecision::mpfr_float HelperFunctionLog(boost::multiprecision::mpfr_float x);
boost::multiprecision::mpfr_float HelperFunctionSqrt(boost::multiprecision::mpfr_float x);
boost::multiprecision::mpfr_float HelperFunctionSin(boost::multiprecision::mpfr_float x);
boost::multiprecision::mpfr_float HelperFunctionAsin(boost::multiprecision::mpfr_float x);
boost::multiprecision::mpfr_float HelperFunctionCos(boost::multiprecision::mpfr_float x);
boost::multiprecision::mpfr_float HelperFunctionAcos(boost::multiprecision::mpfr_float x);
boost::multiprecision::mpfr_float HelperFunctionTan(boost::multiprecision::mpfr_float x);
boost::multiprecision::mpfr_float HelperFunctionAtan(boost::multiprecision::mpfr_float x);
boost::multiprecision::mpfr_float HelperFunctionPow(boost::multiprecision::mpfr_float a, boost::multiprecision::mpfr_float b);

} /* namespace MathOps */

#endif /* MPFRHELPER_H */
