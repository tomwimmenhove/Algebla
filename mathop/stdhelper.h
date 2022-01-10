#ifndef STDHELPER_H
#define STDHELPER_H

#include <numbers>

namespace MathOps
{

template<typename T>
T get_constant_pi() { return std::numbers::pi_v<T>; }

template<typename T>
T get_constant_e() { return std::numbers::e_v<T>; }

float modf(float x, float& integral);
float isnan(float x);
float abs(float x);

float log(float x);
float sqrt(float x);
float sin(float x);
float asin(float x);
float cos(float x);
float acos(float x);
float tan(float x);
float atan(float x);
float pow(float a, float b);


double modf(double x, double& integral);
double isnan(double x);
double abs(double x);

double log(double x);
double sqrt(double x);
double sin(double x);
double asin(double x);
double cos(double x);
double acos(double x);
double tan(double x);
double atan(double x);
double pow(double a, double b);


long double modf(long double x, long double& integral);
long double isnan(long double x);
long double abs(long double x);

long double log(long double x);
long double sqrt(long double x);
long double sin(long double x);
long double asin(long double x);
long double cos(long double x);
long double acos(long double x);
long double tan(long double x);
long double atan(long double x);
long double pow(long double a, long double b);

} /* namespace MathOps */

#endif /* STDHELPER_H */
