#ifndef CONFIG_H
#define CONFIG_H

#ifdef ARBIT_PREC
#include <boost/multiprecision/mpfr.hpp>

typedef boost::multiprecision::mpfr_float number;
#else
typedef long double number;
#endif

#define ALGEBLA_VERSION "Algebla-0.0.1"

#endif /* CONFIG_H */