#ifndef STDHELPER_H
#define STDHELPER_H

#include <numbers>

namespace MathOps
{

template<typename T>
T MathOpFunctionGetConstantPi() { return std::numbers::pi_v<T>; }

template<typename T>
T MathOpFunctionGetConstantE() { return std::numbers::e_v<T>; }


float MathOpFunctionModf(float x, float& integral);
float MathOpFunctionIsnan(float x);
float MathOpFunctionAbs(float x);

float MathOpFunctionLog(float x);
float MathOpFunctionSqrt(float x);
float MathOpFunctionSin(float x);
float MathOpFunctionAsin(float x);
float MathOpFunctionCos(float x);
float MathOpFunctionAcos(float x);
float MathOpFunctionTan(float x);
float MathOpFunctionAtan(float x);
float MathOpFunctionPow(float a, float b);

double MathOpFunctionModf(double x, double& integral);
double MathOpFunctionIsnan(double x);
double MathOpFunctionAbs(double x);

double MathOpFunctionLog(double x);
double MathOpFunctionSqrt(double x);
double MathOpFunctionSin(double x);
double MathOpFunctionAsin(double x);
double MathOpFunctionCos(double x);
double MathOpFunctionAcos(double x);
double MathOpFunctionTan(double x);
double MathOpFunctionAtan(double x);
double MathOpFunctionPow(double a, double b);

long double MathOpFunctionModf(long double x, long double& integral);
long double MathOpFunctionIsnan(long double x);
long double MathOpFunctionAbs(long double x);

long double MathOpFunctionLog(long double x);
long double MathOpFunctionSqrt(long double x);
long double MathOpFunctionSin(long double x);
long double MathOpFunctionAsin(long double x);
long double MathOpFunctionCos(long double x);
long double MathOpFunctionAcos(long double x);
long double MathOpFunctionTan(long double x);
long double MathOpFunctionAtan(long double x);
long double MathOpFunctionPow(long double a, long double b);

} /* namespace MathOps */

#endif /* STDHELPER_H */
