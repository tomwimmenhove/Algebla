#include "stdhelper.h"

#include <cmath>

float MathOpFunctionModf(float x, float& integral) { return std::modf(x, &integral); }
float MathOpFunctionIsnan(float x) { return std::isnan(x); }
float MathOpFunctionAbs(float x) { return std::abs(x); }

float MathOpFunctionLog(float x) { return std::log(x); }
float MathOpFunctionSqrt(float x) { return std::sqrt(x); }
float MathOpFunctionSin(float x) { return std::sin(x); }
float MathOpFunctionAsin(float x) { return std::asin(x); }
float MathOpFunctionCos(float x) { return std::cos(x); }
float MathOpFunctionAcos(float x) { return std::acos(x); }
float MathOpFunctionTan(float x) { return std::tan(x); }
float MathOpFunctionAtan(float x) { return std::atan(x); }
float MathOpFunctionPow(float a, float b) { return std::pow(a, b); }


double MathOpFunctionModf(double x, double& integral) { return std::modf(x, &integral); }
double MathOpFunctionIsnan(double x) { return std::isnan(x); }
double MathOpFunctionAbs(double x) { return std::abs(x); }

double MathOpFunctionLog(double x) { return std::log(x); }
double MathOpFunctionSqrt(double x) { return std::sqrt(x); }
double MathOpFunctionSin(double x) { return std::sin(x); }
double MathOpFunctionAsin(double x) { return std::asin(x); }
double MathOpFunctionCos(double x) { return std::cos(x); }
double MathOpFunctionAcos(double x) { return std::acos(x); }
double MathOpFunctionTan(double x) { return std::tan(x); }
double MathOpFunctionAtan(double x) { return std::atan(x); }
double MathOpFunctionPow(double a, double b) { return std::pow(a, b); }


long double MathOpFunctionModf(long double x, long double& integral) { return std::modf(x, &integral); }
long double MathOpFunctionIsnan(long double x) { return std::isnan(x); }
long double MathOpFunctionAbs(long double x) { return std::abs(x); }

long double MathOpFunctionLog(long double x) { return std::log(x); }
long double MathOpFunctionSqrt(long double x) { return std::sqrt(x); }
long double MathOpFunctionSin(long double x) { return std::sin(x); }
long double MathOpFunctionAsin(long double x) { return std::asin(x); }
long double MathOpFunctionCos(long double x) { return std::cos(x); }
long double MathOpFunctionAcos(long double x) { return std::acos(x); }
long double MathOpFunctionTan(long double x) { return std::tan(x); }
long double MathOpFunctionAtan(long double x) { return std::atan(x); }
long double MathOpFunctionPow(long double a, long double b) { return std::pow(a, b); }
