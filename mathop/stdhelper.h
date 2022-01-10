#ifndef STDHELPER_H
#define STDHELPER_H

#include <numbers>

namespace MathOps
{

template<typename T>
T HelperFunctionGetConstantPi() { return std::numbers::pi_v<T>; }

template<typename T>
T HelperFunctionGetConstantE() { return std::numbers::e_v<T>; }


float HelperFunctionModf(float x, float& integral);
float HelperFunctionIsnan(float x);
float HelperFunctionAbs(float x);

float HelperFunctionLog(float x);
float HelperFunctionSqrt(float x);
float HelperFunctionSin(float x);
float HelperFunctionAsin(float x);
float HelperFunctionCos(float x);
float HelperFunctionAcos(float x);
float HelperFunctionTan(float x);
float HelperFunctionAtan(float x);
float HelperFunctionPow(float a, float b);

double HelperFunctionModf(double x, double& integral);
double HelperFunctionIsnan(double x);
double HelperFunctionAbs(double x);

double HelperFunctionLog(double x);
double HelperFunctionSqrt(double x);
double HelperFunctionSin(double x);
double HelperFunctionAsin(double x);
double HelperFunctionCos(double x);
double HelperFunctionAcos(double x);
double HelperFunctionTan(double x);
double HelperFunctionAtan(double x);
double HelperFunctionPow(double a, double b);

long double HelperFunctionModf(long double x, long double& integral);
long double HelperFunctionIsnan(long double x);
long double HelperFunctionAbs(long double x);

long double HelperFunctionLog(long double x);
long double HelperFunctionSqrt(long double x);
long double HelperFunctionSin(long double x);
long double HelperFunctionAsin(long double x);
long double HelperFunctionCos(long double x);
long double HelperFunctionAcos(long double x);
long double HelperFunctionTan(long double x);
long double HelperFunctionAtan(long double x);
long double HelperFunctionPow(long double a, long double b);

} /* namespace MathOps */

#endif /* STDHELPER_H */
