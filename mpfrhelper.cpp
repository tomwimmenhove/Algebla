#include "mpfrhelper.h"

#include <mpreal.h>

mpfr::mpreal MathOpFunctionModf(mpfr::mpreal x, mpfr::mpreal& integral) { return mpfr::modf(x, integral); }
mpfr::mpreal MathOpFunctionIsnan(mpfr::mpreal x) { return mpfr::isnan(x); }
mpfr::mpreal MathOpFunctionAbs(mpfr::mpreal x) { return mpfr::abs(x); }

mpfr::mpreal MathOpFunctionLog(mpfr::mpreal x) { return mpfr::log(x); }
mpfr::mpreal MathOpFunctionSqrt(mpfr::mpreal x) { return mpfr::sqrt(x); }
mpfr::mpreal MathOpFunctionSin(mpfr::mpreal x) { return mpfr::sin(x); }
mpfr::mpreal MathOpFunctionAsin(mpfr::mpreal x) { return mpfr::asin(x); }
mpfr::mpreal MathOpFunctionCos(mpfr::mpreal x) { return mpfr::cos(x); }
mpfr::mpreal MathOpFunctionAcos(mpfr::mpreal x) { return mpfr::acos(x); }
mpfr::mpreal MathOpFunctionTan(mpfr::mpreal x) { return mpfr::tan(x); }
mpfr::mpreal MathOpFunctionAtan(mpfr::mpreal x) { return mpfr::atan(x); }
mpfr::mpreal MathOpFunctionPow(mpfr::mpreal a, mpfr::mpreal b) { return mpfr::pow(a, b); }
