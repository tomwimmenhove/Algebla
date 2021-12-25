#include <cmath>
#include <limits>
#include <iostream>
#include <string>
#include <functional>
#include <sstream>
#include <iomanip>
#include <numbers>

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

    // std::string str()
    // {
    //     std::stringstream ss;
    //     ss << std::setprecision(30) << numerator << " / " << std::setprecision(30) << denominator;
    //     return ss.str();
    // }

    static Fraction<T> quiet_NaN()
    {
        return Fraction<T>(std::numeric_limits<T>::quiet_NaN(), std::numeric_limits<T>::quiet_NaN());
    }

    static Fraction<T> find(T value, T max_error, int iters)
    {
        Fraction<T> lower(0, 1);
        Fraction<T> upper(1, 0);

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

std::string parenthesize(std::string s)
{
    std::stringstream ss;
    ss << '(' << s << ')';
    return ss.str();
}

template<typename T>
struct UsefulFraction
{
    std::string format;
    std::function<T(T)> operate;
};

template<typename T>
struct MathOp
{
    virtual T get() const = 0;
    virtual int order() const = 0;
    virtual std::string to_string() const = 0;
};

template<typename T>
struct MathOpConstant : public MathOp<T>
{
    MathOpConstant(T c)
        : c(c)
    { }

    T get() const override { return c; }
    int order() const override { return 0; }
    std::string to_string() const
    {
        std::stringstream ss;
        ss << c;
        return ss.str();
    }

private:
    T c;
};

template<typename T>
struct MathOpMul : public MathOp<T>
{
    MathOpMul(MathOp<T>& a, MathOp<T>& b)
        : a(a), b(b)
    { }

    T get() const override { return a.get() * b.get(); }
    int order() const override { return 10; }
    std::string to_string() const
    {
        std::string sa = a.order() > order() ? parenthesize(a.to_string()): a.to_string();
        if (b.get() == 1)
        {
            return sa;
        }

        std::string sb = b.order() > order() ? parenthesize(b.to_string()): b.to_string();
        if (a.get() == 1)
        {
            return sb;
        }

        std::stringstream ss;
        ss << sa << " ⋅ " << sb;
        return ss.str();
    }

private:
    MathOp<T>& a;
    MathOp<T>& b;
};

template<typename T>
struct MathOpAdd : public MathOp<T>
{
    MathOpAdd(MathOp<T>& a, MathOp<T>& b)
        : a(a), b(b)
    { }

    T get() const override { return a.get() + b.get(); }
    int order() const override { return 20; }
    std::string to_string() const
    {
        std::stringstream ss;
        std::string sa = a.order() > order() ? parenthesize(a.to_string()): a.to_string();
        std::string sb = b.order() > order() ? parenthesize(b.to_string()): b.to_string();
        ss << sa << " + " << sb;
        return ss.str();
    }

private:
    MathOp<T>& a;
    MathOp<T>& b;
};

int main(int, char**)
{
    MathOpConstant<double> a(21);
    MathOpConstant<double> b(2);
    MathOpConstant<double> c(.5);
    MathOpAdd<double> d(b, c);
    MathOpMul<double> e(a, d);

    std::cout << e.to_string() << " = " << e.get() << '\n';

    return 0 ;
    std::array<UsefulFraction<double>, 7> uses
    {
        (UsefulFraction<double> {  "(%g) / (%g)",  [](double x) { return x; }}),

        (UsefulFraction<double> { "π(%g) / (%g)",  [](double x) { return x / M_PI; }}),
        (UsefulFraction<double> {  "(%g) / (π%g)", [](double x) { return x * M_PI; }}),

        (UsefulFraction<double> { "e(%g) / (%g)",  [](double x) { return x / M_E; }}),
        (UsefulFraction<double> {  "(%g) / (e%g)", [](double x) { return x * M_E; }}),

        (UsefulFraction<double> { "√2⋅(%g) / (%g)",  [](double x) { return x / M_SQRT2; }}),
        (UsefulFraction<double> {  "(%g) / (√2⋅%g)", [](double x) { return x * M_SQRT2; }}),
    };

    double value = 2.0 / 3.0 / std::sin(M_PI / 4);//2.0 / 3.0 * M_PI;//19.0 / 30.0 * M_PI;

    for(auto i: uses)
    {
        auto x = i.operate(value);
        auto f = Fraction<double>::find(x, 1E-30, 1000);
        if (!f.isnan() && f.numerator < 1000)
        {
            printf(i.format.c_str(), f.numerator, f.denominator);
            printf("\n");
            break;
        }
    }

    return 0;
}
