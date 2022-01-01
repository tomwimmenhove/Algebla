#include "algeblah.h"
#include "findvariabevisitor.h"
#include "replacevisitor.h"
#include "removenoopvisitor.h"

#include <iostream>
#include <functional>

/* Fractions */
template<typename T>
struct Fraction
{
    T numerator;
    T denominator;

    bool is_nan()      const { return std::isnan(numerator) || std::isnan(denominator); }
    bool is_integral() const { return denominator == 1.0; }
    T    fractional()  const { return numerator / denominator; }

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
std::shared_ptr<MathOp<T>> solver(std::shared_ptr<MathOp<T>> y, T value, T max_error, int iters)
{
    auto result = MathFactory::ConstantValue(value);

    auto x = y->accept(new MathOpFindVariableVisitor<T>("x"));
    auto solved = y->solve_for(x, result);
    auto fraction = Fraction<double>::find(solved->result(), max_error, iters);

    if (fraction.is_nan())
    {
        return nullptr;
    }

    return y->accept(new MathOpReplaceVisitor<T>(x, fraction.to_math_op()))
            ->accept(new MathOpRemoveNoOpVisitor<T>());
}

int main(int, char**)
{
    auto x1 = MathFactory::ValueVariable("x", 1.0);
    auto y = log(x1 * MathFactory::SymbolPi<double>() ^ MathFactory::ConstantValue(1.0));
    //auto y = log(MathFactory::ConstantValue(1.0) * MathFactory::SymbolPi<double>() ^ MathFactory::ConstantValue(0.0));

    std::cout << "y = " << *y << " = " << y->result() << '\n';

    y = y->accept(new MathOpRemoveNoOpVisitor<double>());

    auto x = y->accept(new MathOpFindVariableVisitor<double>("x"));
    if (x == x1)
    {
        std::cout << "OK!\n";
    }

    auto r = y->accept(new MathOpReplaceVisitor<double>(x, MathFactory::Variable("Replaced", 1.0)));
    
    std::cout << "r = " << *r << " = " << r->result() << '\n';
    

    std::cout << "x = " << *x << " = " << x->result() << '\n';

    std::cout << "y = " << *y << " = " << y->result() << '\n';

    //return 0;
    double value = M_PI;//15.0 / 4.0 * M_PI;
    //double value = 15.0 / 4.0 / M_PI;

    std::array<std::shared_ptr<MathOp<double>>, 2> equations
    {
        MathFactory::ValueVariable("x", 1.0) * MathFactory::SymbolPi<double>(),
        MathFactory::ValueVariable("x", 1.0) / MathFactory::SymbolPi<double>()
    };

    for (auto y: equations)
    {
        auto solved = solver(y, value, 1E-10, 1000);

        if (solved)
        {
            std::cout << *solved << " = " << solved->result() << '\n';
        }

    }

    return 0;
}
