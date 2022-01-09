#include "mpfrhelper.h"

boost::multiprecision::mpfr_float MathOpFunctionModf(boost::multiprecision::mpfr_float x, boost::multiprecision::mpfr_float &integral) { return boost::multiprecision::modf(x, &integral); }
boost::multiprecision::mpfr_float MathOpFunctionIsnan(boost::multiprecision::mpfr_float x) { return boost::multiprecision::isnan(x); }
boost::multiprecision::mpfr_float MathOpFunctionAbs(boost::multiprecision::mpfr_float x) { return boost::multiprecision::abs(x); }

boost::multiprecision::mpfr_float MathOpFunctionLog(boost::multiprecision::mpfr_float x) { return boost::multiprecision::log(x); }
boost::multiprecision::mpfr_float MathOpFunctionSqrt(boost::multiprecision::mpfr_float x) { return boost::multiprecision::sqrt(x); }
boost::multiprecision::mpfr_float MathOpFunctionSin(boost::multiprecision::mpfr_float x) { return boost::multiprecision::sin(x); }
boost::multiprecision::mpfr_float MathOpFunctionAsin(boost::multiprecision::mpfr_float x) { return boost::multiprecision::asin(x); }
boost::multiprecision::mpfr_float MathOpFunctionCos(boost::multiprecision::mpfr_float x) { return boost::multiprecision::cos(x); }
boost::multiprecision::mpfr_float MathOpFunctionAcos(boost::multiprecision::mpfr_float x) { return boost::multiprecision::acos(x); }
boost::multiprecision::mpfr_float MathOpFunctionTan(boost::multiprecision::mpfr_float x) { return boost::multiprecision::tan(x); }
boost::multiprecision::mpfr_float MathOpFunctionAtan(boost::multiprecision::mpfr_float x) { return boost::multiprecision::atan(x); }
boost::multiprecision::mpfr_float MathOpFunctionPow(boost::multiprecision::mpfr_float a, boost::multiprecision::mpfr_float b) { return boost::multiprecision::pow(a, b); }
