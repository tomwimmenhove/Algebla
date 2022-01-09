#ifndef MPFRHELPER_H
#define MPFRHELPER_H

#include <mpreal.h>

template<typename T>
T MathOpFunctionGetConstantPi() { return (T) mpfr::const_pi(); }

// Apparently, mpfr thinks that e == 0.57722
mpfr::mpreal mprf_get_actual_euler();

template<typename T>
//T MathOpFunctionGetConstantE() { return (T) mpfr::const_euler(); }
T MathOpFunctionGetConstantE() { return (T) mprf_get_actual_euler(); }

mpfr::mpreal MathOpFunctionModf(mpfr::mpreal x, mpfr::mpreal& integral);
mpfr::mpreal MathOpFunctionIsnan(mpfr::mpreal x);
mpfr::mpreal MathOpFunctionAbs(mpfr::mpreal x);

mpfr::mpreal MathOpFunctionLog(mpfr::mpreal x);
mpfr::mpreal MathOpFunctionSqrt(mpfr::mpreal x);
mpfr::mpreal MathOpFunctionSin(mpfr::mpreal x);
mpfr::mpreal MathOpFunctionAsin(mpfr::mpreal x);
mpfr::mpreal MathOpFunctionCos(mpfr::mpreal x);
mpfr::mpreal MathOpFunctionAcos(mpfr::mpreal x);
mpfr::mpreal MathOpFunctionTan(mpfr::mpreal x);
mpfr::mpreal MathOpFunctionAtan(mpfr::mpreal x);
mpfr::mpreal MathOpFunctionPow(mpfr::mpreal a, mpfr::mpreal b);

#endif /* MPFRHELPER_H */
