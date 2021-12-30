#include <cmath>
#include <limits>
#include <iostream>
#include <string>
#include <functional>
#include <sstream>
#include <iomanip>
#include <numbers>
#include <vector>
#include <memory>

/* Forward declarations */
template<typename T> struct MathOpAdd;
template<typename T> struct MathOpSub;
template<typename T> struct MathOpMul;
template<typename T> struct MathOpDiv;
template<typename T> struct MathOpPow;

/* Math operation class base class */
template<typename T>
struct MathOp
{
    virtual T result() const = 0;
    virtual int precedence() const = 0;
    virtual bool is_commutative() const = 0;
    virtual bool is_constant() const = 0;
    virtual std::shared_ptr<MathOp<T>> rearranged(
        std::shared_ptr<MathOp<T>> for_side, std::shared_ptr<MathOp<T>> from) const = 0;

    virtual std::shared_ptr<MathOp<T>> solve_for(std::shared_ptr<MathOp<T>> op, std::shared_ptr<MathOp<T>> from) const
    {
        return op.get() == this ? from : nullptr;
    }

    std::string parenthesize(int parent_precedence, bool use_commutation) const
    {
        std::stringstream ss;

        bool use_parens = !use_commutation || is_commutative()
            ? parent_precedence < precedence()
            : parent_precedence <= precedence();

        if (use_parens)
        {
            ss << '(';
        }

        to_stream(ss);

        if (use_parens)
        {
            ss << ')';
        }

        return ss.str();
    }

    friend std::ostream& operator<<(std::ostream &stream, const MathOp<T> &op)
    {
        return stream << op.parenthesize(std::numeric_limits<int>::max(), false);
    }

    friend std::shared_ptr<MathOp<T>> operator+(std::shared_ptr<MathOp<T>> lhs, std::shared_ptr<MathOp<T>> rhs)
    {
        return std::make_shared<MathOpAdd<T>>(lhs, rhs);
    }

    friend std::shared_ptr<MathOp<T>> operator-(std::shared_ptr<MathOp<T>> lhs, std::shared_ptr<MathOp<T>> rhs)
    {
        return std::make_shared<MathOpSub<T>>(lhs, rhs);
    }

    friend std::shared_ptr<MathOp<T>> operator*(std::shared_ptr<MathOp<T>> lhs, std::shared_ptr<MathOp<T>> rhs)
    {
        return std::make_shared<MathOpMul<T>>(lhs, rhs);
    }

    friend std::shared_ptr<MathOp<T>> operator/(std::shared_ptr<MathOp<T>> lhs, std::shared_ptr<MathOp<T>> rhs)
    {
        return std::make_shared<MathOpDiv<T>>(lhs, rhs);
    }

    friend std::shared_ptr<MathOp<T>> operator^(std::shared_ptr<MathOp<T>> lhs, std::shared_ptr<MathOp<T>> rhs)
    {
        return std::make_shared<MathOpPow<T>>(lhs, rhs);
    }

    virtual ~MathOp() { }

protected:
    virtual std::ostream& to_stream(std::ostream& stream) const = 0;
};

/* Formatters used to display unary operations */
template<typename T>
struct MathUnaryFormatter
{
    virtual std::ostream& to_stream(std::ostream& stream, const MathOp<T>& op,
        std::shared_ptr<MathOp<T>> x) const = 0;
};

template<typename T>
struct MathUnaryFunctionFormatter : public MathUnaryFormatter<T>
{
    MathUnaryFunctionFormatter(std::string name) : name(name) { }
    
    std::ostream& to_stream(std::ostream& stream, const MathOp<T>& op,
        std::shared_ptr<MathOp<T>> x) const override
    {
        return stream << name << '(' << *x << ')';
    }

private:
    std::string name;
};

template<typename T>
struct MathUnaryPostfixFormatter : public MathUnaryFormatter<T>
{
    MathUnaryPostfixFormatter(std::string postfix) : postfix(postfix) { }
    
    std::ostream& to_stream(std::ostream& stream, const MathOp<T>& op,
        std::shared_ptr<MathOp<T>> x) const override
    {
        return stream << '(' << *x << ')' << postfix;
    }

private:
    std::string postfix;
};

/* Formatters used to display binary operations */
template<typename T>
struct MathBinaryFormatter
{
    virtual std::ostream& to_stream(std::ostream& stream, const MathOp<T>& op,
        std::shared_ptr<MathOp<T>> lhs, std::shared_ptr<MathOp<T>> rhs) const = 0;
};

template<typename T>
struct MathBinaryOperationFormatter : public MathBinaryFormatter<T>
{
    MathBinaryOperationFormatter(std::string symbol) : symbol(symbol) { }
    
    std::ostream& to_stream(std::ostream& stream, const MathOp<T>& op,
        std::shared_ptr<MathOp<T>> lhs, std::shared_ptr<MathOp<T>> rhs) const override
    {
        int precedence = op.precedence();
        return stream << lhs->parenthesize(precedence, false) << symbol << rhs->parenthesize(precedence, true);
    }

private:
    std::string symbol;
};

/* Unary math operation base class */
template<typename T, typename U>
struct MathUnaryOp : public MathOp<T>
{
    MathUnaryOp(std::shared_ptr<MathOp<T>> x, int precedence, std::unique_ptr<MathUnaryFormatter<T>> formatter)
        : x(x), prec(precedence), formatter(std::move(formatter))
    { }

    T result() const override { return f(x->result()); }
    int precedence() const override { return prec; }
    bool is_commutative() const override { return true; }
    bool is_constant() const override { return false; }

    std::shared_ptr<MathOp<T>>solve_for(std::shared_ptr<MathOp<T>>op, std::shared_ptr<MathOp<T>>from) const override
    {
        return this->x->solve_for(op, this->rearranged(x, from));
    }

    std::ostream& to_stream(std::ostream& stream) const override
    {
        return formatter->to_stream(stream, *this, x);
    }

protected:
    std::shared_ptr<MathOp<T>>x;
    int prec;
    std::unique_ptr<MathUnaryFormatter<T>> formatter;
    U f;
};

/* Binary math operation base class */
template<typename T, typename U>
struct MathBinaryOp : public MathOp<T>
{
    MathBinaryOp(std::shared_ptr<MathOp<T>>lhs, std::shared_ptr<MathOp<T>>rhs, int precedence,
        std::unique_ptr<MathBinaryFormatter<T>> formatter)
        : lhs(lhs), rhs(rhs), prec(precedence), formatter(std::move(formatter))
    { }

    T result() const override { return f(lhs->result(), rhs->result()); }
    int precedence() const override { return prec; }
    bool is_constant() const override { return false; }

    std::shared_ptr<MathOp<T>>solve_for(std::shared_ptr<MathOp<T>>op, std::shared_ptr<MathOp<T>>from) const override
    {
        auto from_lhs = this->rearranged(lhs, from);

        auto solved_lhs = this->lhs->solve_for(op, from_lhs);
        if (solved_lhs != nullptr)
        {
            return solved_lhs;
        }

        auto from_rhs = this->rearranged(rhs, from);

        return this->rhs->solve_for(op, from_rhs);
    }

    std::ostream& to_stream(std::ostream& stream) const override
    {
        return formatter->to_stream(stream, *this, lhs, rhs);
    }

protected:
    std::shared_ptr<MathOp<T>>lhs;
    std::shared_ptr<MathOp<T>>rhs;
    int prec;
    std::unique_ptr<MathBinaryFormatter<T>> formatter;
    U f;
};

/* Primitive math values */
template<typename T>
struct MathOpSymbol : public MathOp<T>
{
    MathOpSymbol(std::string symbol, T c, bool is_constant)
        : symbol(symbol), c(c), is_const(is_constant)
    { }

    T result() const override { return c; }
    int precedence() const override { return 0; }
    bool is_commutative() const override { return true; }
    bool is_constant() const override { return is_const; }
    std::shared_ptr<MathOp<T>> rearranged(std::shared_ptr<MathOp<T>>, std::shared_ptr<MathOp<T>>) const override { return nullptr; }
    std::ostream& to_stream(std::ostream& stream) const { return stream << symbol; }

protected:
    T c;

private:
    std::string symbol;
    bool is_const;
};

template<typename T>
static std::shared_ptr<MathOpSymbol<T>> MathOpSymbolPi() { return std::make_shared<MathOpSymbol<T>>("π", M_PI, true); }

template<typename T>
static std::shared_ptr<MathOpSymbol<T>> MathOpSymbolE() { return std::make_shared<MathOpSymbol<T>>("e", M_E, true); }

template<typename T>
static std::shared_ptr<MathOpSymbol<T>> MathOpSymbolSqrt2() { return std::make_shared<MathOpSymbol<T>>("√(2)", M_SQRT2, true); }

template <typename T>
static std::shared_ptr<MathOpSymbol<T>> MathOpVariable(std::string symbol, T c = 0)
{
    return std::make_shared<MathOpSymbol<T>>(symbol, c, true);
}

template <typename T>
static std::shared_ptr<MathOpSymbol<T>> MathOpConstantValue(T x)
{
    std::stringstream ss;
    ss << x;
    return std::make_shared<MathOpSymbol<T>>(ss.str(), x, true);
}

/* Unary operation helpers */
template <typename T>
struct logarithm : public std::unary_function<T, T>
{
    T operator()(T x) const
    {
        return std::log(x);
    }
};

template <typename T>
struct square_root : public std::unary_function<T, T>
{
    T operator()(T x) const
    {
        return std::sqrt(x);
    }
};

template <typename T>
struct squares : public std::unary_function<T, T>
{
    T operator()(T x) const
    {
        return x * x;
    }
};

/* Binary operation helpers */
template <typename T>
struct raises : public std::binary_function<T, T, T>
{
    T operator()(T x, T y) const
    {
        return std::pow(x, y);
    }
};

/* Unary math operations */
template<typename T> struct MathOpSquares;

template<typename T>
struct MathOpSqrt : public MathUnaryOp<T, square_root<T>>
{
    MathOpSqrt(std::shared_ptr<MathOp<T>>x)
        : MathUnaryOp<T, square_root<T>>(x, 2, std::make_unique<MathUnaryFunctionFormatter<T>>("√"))
    { }

    std::shared_ptr<MathOp<T>>rearranged(std::shared_ptr<MathOp<T>> for_side, std::shared_ptr<MathOp<T>>from) const override
    {
        if (for_side != this->x) return nullptr;
        return std::make_shared<MathOpSquares<T>>(from);
    }
};

template<typename T>
std::shared_ptr<MathOp<T>> sqrt(std::shared_ptr<MathOp<T>> x) { return std::make_shared<MathOpSqrt<double>>(x); }

template<typename T>
struct MathOpSquares : public MathUnaryOp<T, squares<T>>
{
    MathOpSquares(std::shared_ptr<MathOp<T>>x)
        : MathUnaryOp<T, squares<T>>(x, 2, std::make_unique<MathUnaryPostfixFormatter<T>>("²"))
    { }

    std::shared_ptr<MathOp<T>>rearranged(std::shared_ptr<MathOp<T>> for_side, std::shared_ptr<MathOp<T>>from) const override
    {
        if (for_side != this->x) return nullptr;
        return std::make_shared<MathOpSqrt<T>>(from);
    }
};

template<typename T>
std::shared_ptr<MathOp<T>> square(std::shared_ptr<MathOp<T>> x) { return std::make_shared<MathOpSquares<double>>(x); }

template<typename T>
struct MathOpLog : public MathUnaryOp<T, logarithm<T>>
{
    MathOpLog(std::shared_ptr<MathOp<T>>x)
        : MathUnaryOp<T, logarithm<T>>(x, 2, std::make_unique<MathUnaryFunctionFormatter<T>>("log"))
    { }

    std::shared_ptr<MathOp<T>>rearranged(std::shared_ptr<MathOp<T>> for_side, std::shared_ptr<MathOp<T>>from) const override
    {
        if (for_side != this->x) return nullptr;
        return std::make_shared<MathOpPow<T>>(
            MathOpSymbolE<T>(),
            from
        );
    }
};

template<typename T>
std::shared_ptr<MathOp<T>> log(std::shared_ptr<MathOp<T>> x) { return std::make_shared<MathOpLog<double>>(x); }

/* Binary math operations */
template<typename T>
struct MathOpPow : public MathBinaryOp<T, raises<T>>
{
    MathOpPow(std::shared_ptr<MathOp<T>>lhs, std::shared_ptr<MathOp<T>>rhs)
        : MathBinaryOp<T, raises<T>>(lhs, rhs, 2, std::make_unique<MathBinaryOperationFormatter<T>>(" ^ "))
    { }

    bool is_commutative() const override { return false; }

    std::shared_ptr<MathOp<T>>rearranged(std::shared_ptr<MathOp<T>> for_side, std::shared_ptr<MathOp<T>>from) const override
    {
        if (for_side == this->lhs)
        {
            // if (this->rhs->is_constant() && this->rhs->result() == 2)
            // {
            //     return std::make_shared<MathOpSqrt<T>>(from);
            // }

            return std::make_shared<MathOpPow<T>>(
                from,
                std::make_shared<MathOpDiv<T>>(
                    MathOpConstantValue(1.0),
                    this->rhs
                )
            );
        }
        else if (for_side == this->rhs)
        {
            return std::make_shared<MathOpDiv<T>>(
                std::make_shared<MathOpLog<T>>(from),
                std::make_shared<MathOpLog<T>>(this->lhs)
            ); 
        }
        else
        {
            return nullptr;
        }
    }
};

template<typename T>
struct MathOpMul : public MathBinaryOp<T, std::multiplies<T>>
{
    MathOpMul(std::shared_ptr<MathOp<T>>lhs, std::shared_ptr<MathOp<T>>rhs)
        : MathBinaryOp<T, std::multiplies<T>>(lhs, rhs, 10, std::make_unique<MathBinaryOperationFormatter<T>>(" * "))
    { }

    bool is_commutative() const override { return true; }

    std::shared_ptr<MathOp<T>>rearranged(std::shared_ptr<MathOp<T>> for_side, std::shared_ptr<MathOp<T>>from) const override
    {
        if      (for_side == this->lhs) return std::make_shared<MathOpDiv<T>>(from, this->rhs);
        else if (for_side == this->rhs) return std::make_shared<MathOpDiv<T>>(from, this->lhs);
        else                            return nullptr;
    }
};

template<typename T>
struct MathOpDiv : public MathBinaryOp<T, std::divides<T>>
{
    MathOpDiv(std::shared_ptr<MathOp<T>>lhs, std::shared_ptr<MathOp<T>>rhs)
        : MathBinaryOp<T, std::divides<T>>(lhs, rhs, 10, std::make_unique<MathBinaryOperationFormatter<T>>(" / "))
    { }

    bool is_commutative() const override { return false; }

    std::shared_ptr<MathOp<T>>rearranged(std::shared_ptr<MathOp<T>> for_side, std::shared_ptr<MathOp<T>>from) const override
    {
        if      (for_side == this->lhs) return std::make_shared<MathOpMul<T>>(from, this->rhs);
        else if (for_side == this->rhs) return std::make_shared<MathOpDiv<T>>(this->lhs, from);
        else                            return nullptr;
    }
};

template<typename T>
struct MathOpAdd : public MathBinaryOp<T, std::plus<T>>
{
    MathOpAdd(std::shared_ptr<MathOp<T>>lhs, std::shared_ptr<MathOp<T>>rhs)
        : MathBinaryOp<T, std::plus<T>>(lhs, rhs, 100, std::make_unique<MathBinaryOperationFormatter<T>>(" + "))
    { }

    bool is_commutative() const override { return true; }

    std::shared_ptr<MathOp<T>>rearranged(std::shared_ptr<MathOp<T>> for_side, std::shared_ptr<MathOp<T>>from) const override
    {
        if      (for_side == this->lhs) return std::make_shared<MathOpSub<T>>(from, this->rhs);
        else if (for_side == this->rhs) return std::make_shared<MathOpSub<T>>(from, this->lhs);
        else                            return nullptr;
    }
};

template<typename T>
struct MathOpSub : public MathBinaryOp<T, std::minus<T>>
{
    MathOpSub(std::shared_ptr<MathOp<T>>lhs, std::shared_ptr<MathOp<T>>rhs)
        : MathBinaryOp<T, std::minus<T>>(lhs, rhs, 100, std::make_unique<MathBinaryOperationFormatter<T>>(" - "))
    { }

    bool is_commutative() const override { return false; }

    std::shared_ptr<MathOp<T>>rearranged(std::shared_ptr<MathOp<T>> for_side, std::shared_ptr<MathOp<T>>from) const override
    {
        if      (for_side == this->lhs) return std::make_shared<MathOpAdd<T>>(from, this->rhs);
        else if (for_side == this->rhs) return std::make_shared<MathOpSub<T>>(this->lhs, from);
        else                            return nullptr;
    }
};

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
struct UsefulFraction
{
    std::string format;
    std::function<T(T)> operate;
};

int main(int, char**)
{
    auto x = MathOpVariable("x", 21.0);
    auto y = sqrt(MathOpSymbolPi<double>() ^ (x * (MathOpConstantValue(2.0) + MathOpSymbolPi<double>())));

    std::cout << "x = " << x->result() << '\n';

    std::cout << "y = " << *y << " = " << y->result() << '\n';

    auto output = MathOpConstantValue(y->result());

    auto q = y->solve_for(x, output);

    std::cout << *x << " = " << *q << " = " << q->result() << '\n';

    auto r = q->solve_for(output, x);

    std::cout << "y = " << *r << " = " << r->result() << '\n';

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

    double value = 15.0 / 4.0;//2.0 / 3.0 / std::sin(M_PI / 4);//2.0 / 3.0 * M_PI;//19.0 / 30.0 * M_PI;

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
