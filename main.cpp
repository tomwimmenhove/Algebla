#include "algeblah.h"
#include "findvariabetransformer.h"
#include "replacetransformer.h"
#include "removenooptransformer.h"
#include "solvertransformer.h"

#include <iostream>
#include <functional>

/* Fractions */
template<typename T>
struct Fraction
{
    T numerator;
    T denominator;

    bool is_nan()      const { return std::isnan(numerator) || std::isnan(denominator); }
    bool is_integral() const { return denominator == 1; }
    T    result()      const { return numerator / denominator; }

    Fraction(T numerator, T denominator)
        : numerator(numerator), denominator(denominator)
    { }

    std::shared_ptr<MathOp<T>> to_math_op() const
    {
        return MathFactory::ConstantValue(numerator) / MathFactory::ConstantValue(denominator);
    }

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
            T test = middle.result();

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
Fraction<T> solver(std::shared_ptr<MathOp<T>> y, std::shared_ptr<MathOp<T>> numerator, T value, T max_error, int iters)
{
    auto result = MathFactory::ConstantValue(value);

    auto solved = y->transform(MathOpSolverTransformer<T>(numerator, result));
    auto fraction = Fraction<double>::find(solved->result(), max_error, iters);

    return fraction;
}

template<typename T>
std::shared_ptr<MathOp<T>> find_fraction(std::vector<std::shared_ptr<MathOp<T>>> equations,
    T value, T max_error, int iters)
{
    auto best_fraction = Fraction<T>::quiet_NaN();
    std::shared_ptr<MathOp<T>> best_numerator,best_denominator, best_y;

    for (auto y: equations)
    {
        auto numerator = y->transform(MathOpFindVariableTransformer<T>("numerator"));

        auto fraction = solver(y, numerator, value, 1E-10, 1000);

        if (!fraction.is_nan() && (best_fraction.is_nan() || fraction.numerator < best_fraction.numerator))
        {
            best_fraction = fraction;
            best_numerator = numerator;
            best_denominator = y->transform(MathOpFindVariableTransformer<T>("denominator"));
            best_y = y;
        }
    }

    if (!best_y)
    {
        return nullptr;
    }

    return best_y->transform(MathOpReplaceTransformer<T>(best_numerator,   MathFactory::ConstantValue(best_fraction.numerator)))
                 ->transform(MathOpReplaceTransformer<T>(best_denominator, MathFactory::ConstantValue(best_fraction.denominator)))
                 ->transform(MathOpRemoveNoOpTransformer<T>());
}

int main(int, char**)
{
    auto numerator = MathFactory::NamedConstant("numerator", 1.0);
    auto denominator = MathFactory::NamedConstant("denominator", 1.0);
    std::vector<std::shared_ptr<MathOp<double>>> equations
    {
        numerator * MathFactory::SymbolPi<double>() / denominator,
        numerator / (MathFactory::SymbolPi<double>() * denominator)
    };

    auto y1 = find_fraction(equations, 15.0 / 4.0 / M_PI, 1E-10, 1000);
    auto y2 = find_fraction(equations, 15.0 / 4.0 * M_PI, 1E-10, 1000);

    std::cout << "y1 = " << *y1 << " = " << y1->result() << '\n';
    std::cout << "y2 = " << *y2 << " = " << y2->result() << '\n';

    return 0;
}

int maidffren(int, char**)
{
    auto x1 = MathFactory::ValueVariable("x", 21.0);

    auto y = sqrt(MathFactory::SymbolPi<double>() ^ (x1 * (MathFactory::ConstantValue(2.0) + MathFactory::SymbolPi<double>())));
    
    auto a = y->transform(MathOpSolverTransformer<double>(x1, MathFactory::ConstantValue(y->result())));

    std::cout << "y = " << *y << " = " << y->result() << '\n';

    std::cout << "a = " << *a << " = " << a->result() << '\n';

    y = y->transform(MathOpRemoveNoOpTransformer<double>());

    auto x = y->transform(MathOpFindVariableTransformer<double>("x"));
    if (x == x1)
    {
        std::cout << "OK!\n";
    }

    auto r = y->transform(MathOpReplaceTransformer<double>(x, MathFactory::Variable("Replaced", 1.0)));
    
    std::cout << "r = " << *r << " = " << r->result() << '\n';
    

    std::cout << "x = " << *x << " = " << x->result() << '\n';

    std::cout << "y = " << *y << " = " << y->result() << '\n';

    return 0;
}
