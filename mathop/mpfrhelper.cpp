#include "mpfrhelper.h"

namespace MathOps
{

boost::multiprecision::mpfr_float HelperFunctionModf(boost::multiprecision::mpfr_float x, boost::multiprecision::mpfr_float &integral) { return boost::multiprecision::modf(x, &integral); }
boost::multiprecision::mpfr_float HelperFunctionIsnan(boost::multiprecision::mpfr_float x) { return boost::multiprecision::isnan(x); }
boost::multiprecision::mpfr_float HelperFunctionAbs(boost::multiprecision::mpfr_float x) { return boost::multiprecision::abs(x); }

boost::multiprecision::mpfr_float HelperFunctionLog(boost::multiprecision::mpfr_float x) { return boost::multiprecision::log(x); }
boost::multiprecision::mpfr_float HelperFunctionSqrt(boost::multiprecision::mpfr_float x) { return boost::multiprecision::sqrt(x); }
boost::multiprecision::mpfr_float HelperFunctionSin(boost::multiprecision::mpfr_float x) { return boost::multiprecision::sin(x); }
boost::multiprecision::mpfr_float HelperFunctionAsin(boost::multiprecision::mpfr_float x) { return boost::multiprecision::asin(x); }
boost::multiprecision::mpfr_float HelperFunctionCos(boost::multiprecision::mpfr_float x) { return boost::multiprecision::cos(x); }
boost::multiprecision::mpfr_float HelperFunctionAcos(boost::multiprecision::mpfr_float x) { return boost::multiprecision::acos(x); }
boost::multiprecision::mpfr_float HelperFunctionTan(boost::multiprecision::mpfr_float x) { return boost::multiprecision::tan(x); }
boost::multiprecision::mpfr_float HelperFunctionAtan(boost::multiprecision::mpfr_float x) { return boost::multiprecision::atan(x); }
boost::multiprecision::mpfr_float HelperFunctionPow(boost::multiprecision::mpfr_float a, boost::multiprecision::mpfr_float b) { return boost::multiprecision::pow(a, b); }

} /* namespace MathOps */
