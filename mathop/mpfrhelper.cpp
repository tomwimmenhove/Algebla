#include "mpfrhelper.h"

namespace MathOps
{

boost::multiprecision::mpfr_float modf(boost::multiprecision::mpfr_float x, boost::multiprecision::mpfr_float &integral) { return boost::multiprecision::modf(x, &integral); }
boost::multiprecision::mpfr_float isnan(boost::multiprecision::mpfr_float x) { return boost::multiprecision::isnan(x); }
boost::multiprecision::mpfr_float abs(boost::multiprecision::mpfr_float x) { return boost::multiprecision::abs(x); }

boost::multiprecision::mpfr_float log(boost::multiprecision::mpfr_float x) { return boost::multiprecision::log(x); }
boost::multiprecision::mpfr_float sqrt(boost::multiprecision::mpfr_float x) { return boost::multiprecision::sqrt(x); }
boost::multiprecision::mpfr_float sin(boost::multiprecision::mpfr_float x) { return boost::multiprecision::sin(x); }
boost::multiprecision::mpfr_float asin(boost::multiprecision::mpfr_float x) { return boost::multiprecision::asin(x); }
boost::multiprecision::mpfr_float cos(boost::multiprecision::mpfr_float x) { return boost::multiprecision::cos(x); }
boost::multiprecision::mpfr_float acos(boost::multiprecision::mpfr_float x) { return boost::multiprecision::acos(x); }
boost::multiprecision::mpfr_float tan(boost::multiprecision::mpfr_float x) { return boost::multiprecision::tan(x); }
boost::multiprecision::mpfr_float atan(boost::multiprecision::mpfr_float x) { return boost::multiprecision::atan(x); }
boost::multiprecision::mpfr_float pow(boost::multiprecision::mpfr_float a, boost::multiprecision::mpfr_float b) { return boost::multiprecision::pow(a, b); }

} /* namespace MathOps */
