#include <cmath>
#include <limits>
#include <iostream>

template<typename T>
struct Fraction
{
    T numerator;
    T denominator;

    bool isnan()   const { return std::isnan(numerator) || std::isnan(denominator); }
    T fractional() const { return numerator / denominator; }

    Fraction(T numerator, T denominator)
        : numerator(numerator), denominator(denominator)
    { }

    static Fraction<T> quiet_NaN()
    {
        return Fraction<T>(std::numeric_limits<double>::quiet_NaN(), std::numeric_limits<double>::quiet_NaN());
    }
};

template<typename T>
Fraction<T> find_fraction(T fractional, T max_error, int iters)
{
    Fraction<T> lower(0, 1);
    Fraction<T> upper(1, 0);

    while (iters--)
    {
        Fraction<T> middle(lower.numerator + upper.numerator, lower.denominator + upper.denominator);
        T test = middle.fractional();

        if (std::abs(test - fractional) <= max_error)
        {
            return middle;
        }

        if (fractional > test)
        {
            lower = middle;
        }
        else
        {
            upper = middle;
        }
    }

    return Fraction<T>::quiet_NaN();
}

int main(int, char**)
{
    auto f = find_fraction<double>(15.0f / 4.0f, 1E-12, 1000000);
    if (!f.isnan())
    {
        std::cout << "Hello: " << f.numerator << '/' << f.denominator << '\n';
    }
    else
    {
        std::cout << "Not found\n";
    }
}
