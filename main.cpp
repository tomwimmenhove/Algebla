#include "algeblah.h"
#include "findvariabetransformer.h"
#include "replacetransformer.h"
#include "removenooptransformer.h"
#include "rearrangetransformer.h"
#include "defaultformatter.h"

#include "config.h"

#include <iostream>
#include <functional>
#include <readline/readline.h>
#include <readline/history.h>

/* Fractions */
template <typename T>
struct Fraction
{
    T numerator;
    T denominator;

    bool is_nan() const { return std::isnan(numerator) || std::isnan(denominator); }
    bool is_integral() const { return denominator == 1; }
    T result() const { return numerator / denominator; }

    Fraction(T numerator, T denominator)
        : numerator(numerator), denominator(denominator)
    {
    }

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

template <typename T>
Fraction<T> solver(std::shared_ptr<MathOp<T>> y, std::shared_ptr<MathOp<T>> numerator, T value, T max_error, int iters)
{
    auto result = MathFactory::ConstantValue(value);

    auto solved = y->transform(MathOpRearrangeTransformer<T>(numerator, result));
    auto fraction = Fraction<number>::find(solved->result(), max_error, iters);

    return fraction;
}

template <typename T>
std::shared_ptr<MathOp<T>> find_fraction(std::vector<std::shared_ptr<MathOp<T>>> equations,
                                         T value, T max_error, int iters)
{
    auto best_fraction = Fraction<T>::quiet_NaN();
    std::shared_ptr<MathOp<T>> best_numerator, best_denominator, best_y;

    for (auto y : equations)
    {
        auto numerator = y->transform(MathOpFindVariableTransformer<T>("numerator"));

        auto fraction = solver<T>(y, numerator, value, 1E-10, 1000);

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

    return best_y->transform(MathOpReplaceTransformer<T>(best_numerator, MathFactory::ConstantValue(best_fraction.numerator)))
        ->transform(MathOpReplaceTransformer<T>(best_denominator, MathFactory::ConstantValue(best_fraction.denominator)))
        ->transform(MathOpRemoveNoOpTransformer<T>());
}

#include "driver.h"

int main(int, char **)
{
    driver drv;
    MathOpDefaultFormatter<number> formatter;

#if 0
    if (drv.parse_string("solve a: 12 + a * 123 == 1012") != 0)
    //if (drv.parse_string("a + 2 + 3") != 0)
    {
        return 1;
    }

    for (auto &exp : drv.get_expressions())
    {
        std::cout << "  " << exp->format(formatter) << " = " << exp->result() << '\n';
    }

    return 0;
#endif

    char *line_buf;
    while ((line_buf = readline("> ")) != nullptr)
    {
        if (strlen(line_buf) > 0)
        {
            add_history(line_buf);
        }

        if (drv.parse_string(line_buf) != 0)
        {
            continue;
        }

        for (auto &exp : drv.get_expressions())
        {
            std::cout << "  " << exp->format(formatter) << " = " << exp->result() << '\n';
        }

        // readline malloc's a new buffer every time.
        free(line_buf);
    }

    return 0;
    const auto numerator = MathFactory::NamedConstant<number>("numerator", 1.0);
    const auto denominator = MathFactory::NamedConstant<number>("denominator", 1.0);
    const auto pi = MathFactory::SymbolPi<number>();
    std::vector<std::shared_ptr<MathOp<number>>> equations{
        numerator * pi / denominator,
        numerator / (pi * denominator)};

    auto y1 = find_fraction<number>(equations, 15.0 / 4.0 / M_PI, 1E-10, 1000);
    auto y2 = find_fraction<number>(equations, 15.0 / 4.0 * M_PI, 1E-10, 1000);

    std::cout << "y1 = " << y1->format(formatter) << " = " << y1->result() << '\n';
    std::cout << "y2 = " << y2->format(formatter) << " = " << y2->result() << '\n';

    std::cout << (pi - (pi + pi))->format(formatter) << '\n';

    return 0;
}

int maidffren(int, char **)
{
    auto x1 = MathFactory::ValueVariable<number>("x", 21.0);

    auto y = sqrt(MathFactory::SymbolPi<number>() ^ (x1 * (MathFactory::ConstantValue<number>(2.0) + MathFactory::SymbolPi<number>())));

    auto a = y->transform(MathOpRearrangeTransformer<number>(x1, MathFactory::ConstantValue(y->result())));

    MathOpDefaultFormatter<number> formatter;

    std::cout << "y = " << y->format(formatter) << " = " << y->result() << '\n';

    std::cout << "a = " << a->format(formatter) << " = " << a->result() << '\n';

    y = y->transform(MathOpRemoveNoOpTransformer<number>());

    auto x = y->transform(MathOpFindVariableTransformer<number>("x"));
    if (x == x1)
    {
        std::cout << "OK!\n";
    }

    auto r = y->transform(MathOpReplaceTransformer<number>(x, MathFactory::Variable<number>("Replaced", 1.0)));

    std::cout << "r = " << r->format(formatter) << " = " << r->result() << '\n';

    std::cout << "x = " << x->format(formatter) << " = " << x->result() << '\n';

    std::cout << "y = " << y->format(formatter) << " = " << y->result() << '\n';

    return 0;
}
