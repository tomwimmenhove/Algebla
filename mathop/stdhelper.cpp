#include "stdhelper.h"

#include <cmath>

namespace MathOps
{

float HelperFunctionModf(float x, float& integral) { return std::modf(x, &integral); }
float HelperFunctionIsnan(float x) { return std::isnan(x); }
float HelperFunctionAbs(float x) { return std::abs(x); }

float HelperFunctionLog(float x) { return std::log(x); }
float HelperFunctionSqrt(float x) { return std::sqrt(x); }
float HelperFunctionSin(float x) { return std::sin(x); }
float HelperFunctionAsin(float x) { return std::asin(x); }
float HelperFunctionCos(float x) { return std::cos(x); }
float HelperFunctionAcos(float x) { return std::acos(x); }
float HelperFunctionTan(float x) { return std::tan(x); }
float HelperFunctionAtan(float x) { return std::atan(x); }
float HelperFunctionPow(float a, float b) { return std::pow(a, b); }


double HelperFunctionModf(double x, double& integral) { return std::modf(x, &integral); }
double HelperFunctionIsnan(double x) { return std::isnan(x); }
double HelperFunctionAbs(double x) { return std::abs(x); }

double HelperFunctionLog(double x) { return std::log(x); }
double HelperFunctionSqrt(double x) { return std::sqrt(x); }
double HelperFunctionSin(double x) { return std::sin(x); }
double HelperFunctionAsin(double x) { return std::asin(x); }
double HelperFunctionCos(double x) { return std::cos(x); }
double HelperFunctionAcos(double x) { return std::acos(x); }
double HelperFunctionTan(double x) { return std::tan(x); }
double HelperFunctionAtan(double x) { return std::atan(x); }
double HelperFunctionPow(double a, double b) { return std::pow(a, b); }


long double HelperFunctionModf(long double x, long double& integral) { return std::modf(x, &integral); }
long double HelperFunctionIsnan(long double x) { return std::isnan(x); }
long double HelperFunctionAbs(long double x) { return std::abs(x); }

long double HelperFunctionLog(long double x) { return std::log(x); }
long double HelperFunctionSqrt(long double x) { return std::sqrt(x); }
long double HelperFunctionSin(long double x) { return std::sin(x); }
long double HelperFunctionAsin(long double x) { return std::asin(x); }
long double HelperFunctionCos(long double x) { return std::cos(x); }
long double HelperFunctionAcos(long double x) { return std::acos(x); }
long double HelperFunctionTan(long double x) { return std::tan(x); }
long double HelperFunctionAtan(long double x) { return std::atan(x); }
long double HelperFunctionPow(long double a, long double b) { return std::pow(a, b); }

} /* namespace MathOps */
