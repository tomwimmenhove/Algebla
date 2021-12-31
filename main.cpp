#include "algeblah.h"

#include <iostream>
#include <functional>

/* Fractions */
template<typename T>
struct Fraction
{
    T numerator;
    T denominator;

    bool isnan()      const { return std::isnan(numerator) || std::isnan(denominator); }
    bool isintegral() const { return denominator == 1.0; }
    T    fractional() const { return numerator / denominator; }

    Fraction(T numerator, T denominator)
        : numerator(numerator), denominator(denominator)
    { }

    static Fraction<T> quiet_NaN()
    {
        return Fraction<T>(std::numeric_limits<T>::quiet_NaN(), std::numeric_limits<T>::quiet_NaN());
    }

    static Fraction<T> find(T value, T max_error, int iters)
    {
        Fraction<T> lower(0, 1);
        Fraction<T> upper(1, 1);

        T integral;
        T fractional = std::modf(value, &integral);

        if (fractional == 0.0)
        {
            return Fraction<T>(integral, 1);
        }

        while (iters--)
        {
            Fraction<T> middle(lower.numerator + upper.numerator, lower.denominator + upper.denominator);
            T test = middle.fractional();

            if (std::abs(test - fractional) <= max_error)
            {
                return Fraction<T>(middle.numerator + integral * middle.denominator, middle.denominator);
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
};

template<typename T>
Fraction<T> solver(std::shared_ptr<MathOp<T>> y,
            std::shared_ptr<MathOpVariable<T>> numerator,
            std::shared_ptr<MathOpVariable<T>> denominator, T value, T max_error, int iters)
{
    auto result = MathFactory::ConstantValue(value);

    auto solved = y->solve_for(numerator, result);
    auto fraction = Fraction<double>::find(solved->result(), max_error, iters);

    numerator->set(fraction.numerator);
    denominator->set(fraction.denominator);

    return fraction;
}

template<typename T>
Fraction<T> solver(std::shared_ptr<MathOp<T>> y, T value, T max_error, int iters)
{
    auto numerator = y->find_variable("numerator");
    auto denominator = y->find_variable("denominator");

    return solver<T>(y, numerator, denominator, value, max_error, iters);
}

int main(int, char**)
{
    //double value = 15.0 / 4.0 * M_PI;
    double value = 15.0 / 4.0 / M_PI;

    std::array<std::shared_ptr<MathOp<double>>, 2> equations
    {
        MathFactory::ValueVariable("numerator", 1.0)
            * MathFactory::SymbolPi<double>()
            / MathFactory::ValueVariable("denominator", 1.0),
        MathFactory::ValueVariable("numerator", 1.0)
            / (MathFactory::SymbolPi<double>()
            * MathFactory::ValueVariable("denominator", 1.0))
    };

    for (auto y: equations)
    {
        auto fraction = solver(y, value, 1E-10, 1000);

        if (!fraction.isnan())
        {
            std::cout << *y << " = " << y->result() << '\n';
        }

    }

    return 0;
}
