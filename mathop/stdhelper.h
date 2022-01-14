#ifndef STDHELPER_H
#define STDHELPER_H

//#include <numbers>
#include <cmath>

namespace MathOps
{

template<typename T>
//T get_constant_pi() { return std::numbers::pi_v<T>; }
T get_constant_pi() { return (T) (M_PIl); }

template<typename T>
//T get_constant_e() { return std::numbers::e_v<T>; }
T get_constant_e() { return (T) (M_El); }

inline float modf(float x, float& integral) { return std::modf(x, &integral); }
inline float isnan(float x) { return std::isnan(x); }
inline float abs(float x) { return std::abs(x); }

inline float log(float x) { return std::log(x); }
inline float sqrt(float x) { return std::sqrt(x); }
inline float sin(float x) { return std::sin(x); }
inline float asin(float x) { return std::asin(x); }
inline float cos(float x) { return std::cos(x); }
inline float acos(float x) { return std::acos(x); }
inline float tan(float x) { return std::tan(x); }
inline float atan(float x) { return std::atan(x); }
inline float pow(float a, float b) { return std::pow(a, b); }


inline double modf(double x, double& integral) { return std::modf(x, &integral); }
inline double isnan(double x) { return std::isnan(x); }
inline double abs(double x) { return std::abs(x); }

inline double log(double x) { return std::log(x); }
inline double sqrt(double x) { return std::sqrt(x); }
inline double sin(double x) { return std::sin(x); }
inline double asin(double x) { return std::asin(x); }
inline double cos(double x) { return std::cos(x); }
inline double acos(double x) { return std::acos(x); }
inline double tan(double x) { return std::tan(x); }
inline double atan(double x) { return std::atan(x); }
inline double pow(double a, double b) { return std::pow(a, b); }


inline long double modf(long double x, long double& integral) { return std::modf(x, &integral); }
inline long double isnan(long double x) { return std::isnan(x); }
inline long double abs(long double x) { return std::abs(x); }

inline long double log(long double x) { return std::log(x); }
inline long double sqrt(long double x) { return std::sqrt(x); }
inline long double sin(long double x) { return std::sin(x); }
inline long double asin(long double x) { return std::asin(x); }
inline long double cos(long double x) { return std::cos(x); }
inline long double acos(long double x) { return std::acos(x); }
inline long double tan(long double x) { return std::tan(x); }
inline long double atan(long double x) { return std::atan(x); }
inline long double pow(long double a, long double b) { return std::pow(a, b); }

} /* namespace MathOps */

#endif /* STDHELPER_H */
