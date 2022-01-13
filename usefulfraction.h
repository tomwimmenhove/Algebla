#ifndef USEFULFRACTION_H
#define USEFULFRACTION_H

#include "defaulthelper.h"
#include "mathop/algeblah.h"
#include "mathop/defaultformatter.h"
#include "mathop/replacetransformer.h"
#include "mathop/removenooptransformer.h"

template <typename T>
struct Fraction
{
    T numerator;
    T denominator;

    bool is_nan() const { return MathOps::isnan(numerator) || MathOps::isnan(denominator); }
    bool is_integral() const { return denominator == 1; }
    T result() const { return numerator / denominator; }

    Fraction(T numerator, T denominator)
        : numerator(numerator), denominator(denominator)
    {
    }

    std::shared_ptr<MathOps::MathOp<T>> to_math_op() const
    {
        return MathOps::Factory::CreateConstantValue(numerator) / MathOps::Factory::CreateConstantValue(denominator);
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
        T fractional = MathOps::modf(value, integral);

        /* XXX: This is a hack */
        // if (MathOps::HelperFunctionAbs(fractional) < max_error)
        // {
        //     fractional = 0;
        // }

        if (fractional == 0.0)
        {
            return Fraction<T>(integral, 1);
        }

        while (iters--)
        {
            Fraction<T> middle(lower.numerator + upper.numerator, lower.denominator + upper.denominator);
            T test = middle.result();

            if (MathOps::abs(test - fractional) <= max_error)
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

template <typename T>
Fraction<T> solver(std::shared_ptr<MathOps::MathOp<T>> y, std::shared_ptr<MathOps::MathOp<T>> numerator, T value, T max_error, int iters)
{
    auto result = MathOps::Factory::CreateConstantValue(value);

    auto solved = y->transform(MathOps::MathOpRearrangeTransformer<T>(numerator, result));
    auto fraction = Fraction<T>::find(solved->result(), max_error, iters);

    return fraction;
}

template <typename T>
std::shared_ptr<MathOps::MathOp<T>> find_fraction(std::vector<std::shared_ptr<MathOps::MathOp<T>>> equations,
                                         T value, T max_error, int iters, T max_num_denominator)
{
    auto best_fraction = Fraction<T>::quiet_NaN();
    std::shared_ptr<MathOps::MathOp<T>> best_numerator, best_denominator, best_y;

    for (auto y : equations)
    {
        auto numerator = y->transform(MathOps::FindNamedValueTransformer<T>("numerator"));

        auto fraction = solver<T>(y, numerator, value, max_error, iters);

        if (!fraction.is_nan() && (best_fraction.is_nan() || fraction.numerator < best_fraction.numerator))
        {
            best_fraction = fraction;
            best_numerator = numerator;
            best_denominator = y->transform(MathOps::FindNamedValueTransformer<T>("denominator"));
            best_y = y;
        }
    }

    if (!best_y || best_fraction.denominator > max_num_denominator || best_fraction.numerator > max_num_denominator)
    {
        return nullptr;
    }

    return best_y->transform(MathOps::ReplaceTransformer<T>(best_numerator, MathOps::Factory::CreateConstantValue(best_fraction.numerator)))
        ->transform(MathOps::ReplaceTransformer<T>(best_denominator, MathOps::Factory::CreateConstantValue(best_fraction.denominator)))
        ->transform(MathOps::MathOpRemoveNoOpTransformer<T>());
}

template<typename T>
std::string useful_fraction(T x)
{
    std::stringstream ss;

    if (x == 0)
    {
        return { };
    }

    const auto numerator = MathOps::Factory::CreateNamedConstant<T>("numerator", 1.0);
    const auto denominator = MathOps::Factory::CreateNamedConstant<T>("denominator", 1.0);
    const auto pi = MathOps::Factory::CreateSymbolPi<T>();
    const auto e = MathOps::Factory::CreateSymbolE<T>();
    const auto sq2 = sqrt<T>(MathOps::Factory::CreateConstantValue<T>(2));
    std::vector<std::shared_ptr<MathOps::MathOp<T>>> equations{
        numerator * pi / denominator,
        numerator / (pi * denominator),
        numerator * e / denominator,
        numerator / (e * denominator),
        numerator * sq2 / denominator,
        numerator / (sq2 * denominator)
    };

    auto y = find_fraction<T>(equations, x, 1E-15, 1000, 10000);

    if (!y)
    {
        return { };
    }

    return y->format(MathOps::DefaultFormatter<T>());
}

#endif /* USEFULFRACTION_H */