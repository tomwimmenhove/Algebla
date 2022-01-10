#include "stdhelper.h"

#include <cmath>

namespace MathOps
{

float modf(float x, float& integral) { return std::modf(x, &integral); }
float isnan(float x) { return std::isnan(x); }
float abs(float x) { return std::abs(x); }

float log(float x) { return std::log(x); }
float sqrt(float x) { return std::sqrt(x); }
float sin(float x) { return std::sin(x); }
float asin(float x) { return std::asin(x); }
float cos(float x) { return std::cos(x); }
float acos(float x) { return std::acos(x); }
float tan(float x) { return std::tan(x); }
float atan(float x) { return std::atan(x); }
float pow(float a, float b) { return std::pow(a, b); }


double modf(double x, double& integral) { return std::modf(x, &integral); }
double isnan(double x) { return std::isnan(x); }
double abs(double x) { return std::abs(x); }

double log(double x) { return std::log(x); }
double sqrt(double x) { return std::sqrt(x); }
double sin(double x) { return std::sin(x); }
double asin(double x) { return std::asin(x); }
double cos(double x) { return std::cos(x); }
double acos(double x) { return std::acos(x); }
double tan(double x) { return std::tan(x); }
double atan(double x) { return std::atan(x); }
double pow(double a, double b) { return std::pow(a, b); }


long double modf(long double x, long double& integral) { return std::modf(x, &integral); }
long double isnan(long double x) { return std::isnan(x); }
long double abs(long double x) { return std::abs(x); }

long double log(long double x) { return std::log(x); }
long double sqrt(long double x) { return std::sqrt(x); }
long double sin(long double x) { return std::sin(x); }
long double asin(long double x) { return std::asin(x); }
long double cos(long double x) { return std::cos(x); }
long double acos(long double x) { return std::acos(x); }
long double tan(long double x) { return std::tan(x); }
long double atan(long double x) { return std::atan(x); }
long double pow(long double a, long double b) { return std::pow(a, b); }

} /* namespace MathOps */
