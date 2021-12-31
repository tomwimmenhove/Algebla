#ifndef ALGEBLAH_H
#define ALGEBLAH_H

#include <cmath>
#include <limits>
#include <string>
#include <sstream>
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
struct MathOpValue : public MathOp<T>
{
    MathOpValue(T value, bool is_constant)
        : value(value), is_const(is_constant)
    { }

    T result() const override { return value; }
    void set(T x) { this->value = x; }
    int precedence() const override { return 0; }
    bool is_commutative() const override { return true; }
    bool is_constant() const override { return is_const; }
    std::shared_ptr<MathOp<T>> rearranged(std::shared_ptr<MathOp<T>>, std::shared_ptr<MathOp<T>>) const override { return nullptr; }
    std::ostream& to_stream(std::ostream& stream) const { return stream << symbol_str(); }

protected:
    T value;

    virtual std::string symbol_str() const
    {
        std::stringstream ss;
        ss << this->value;
        return ss.str();
    }

private:
    bool is_const;
};

template<typename T>
struct MathOpSymbol : public MathOpValue<T>
{
    MathOpSymbol(std::string symbol, T value, bool is_constant)
        : symbol(symbol), MathOpValue<T>(value, is_constant)
    { }

protected:
    std::string symbol_str() const override { return symbol; }

private:
    std::string symbol;
};

template<typename T>
struct MathOpMutableSymbol : public MathOpSymbol<T>
{
    MathOpMutableSymbol(std::string symbol, T value) : MathOpSymbol<T>(symbol, value, false) { }
};

template<typename T>
struct MathOpConstantSymbol : public MathOpSymbol<T>
{
    MathOpConstantSymbol(std::string symbol, T value) : MathOpSymbol<T>(symbol, value, true) { }

    void set(T x) = delete;
};

template<typename T>
struct MathOpVariable : public MathOpMutableSymbol<T>
{
    MathOpVariable(std::string symbol, T x) : MathOpMutableSymbol<T>(symbol, x) { }
};

template<typename T>
struct MathOpMutableValue : public MathOpValue<T>
{
    MathOpMutableValue(T value) : MathOpValue<T>(value, false) { }
};

template<typename T>
struct MathOpConstantValue : public MathOpValue<T>
{
    MathOpConstantValue(T value) : MathOpValue<T>(value, true) { }

    void set(T x) = delete;
};

struct MathFactory
{
    template <typename T>
    static std::shared_ptr<MathOp<T>> SymbolPi() { return std::make_shared<MathOpConstantSymbol<T>>("π", M_PI); }

    template <typename T>
    static std::shared_ptr<MathOp<T>> SymbolE() { return std::make_shared<MathOpConstantSymbol<T>>("e", M_E); }

    template <typename T>
    static std::shared_ptr<MathOp<T>> SymbolSqrt2() { return std::make_shared<MathOpConstantSymbol<T>>("√(2)", M_SQRT2); }

    template <typename T>
    static std::shared_ptr<MathOpVariable<T>> Variable(std::string symbol, T c = 0)
    {
        return std::make_shared<MathOpVariable<T>>(symbol, c);
    }

    template <typename T>
    static std::shared_ptr<MathOpConstantValue<T>> ConstantValue(T c)
    {
        return std::make_shared<MathOpConstantValue<T>>(c);
    }

    template <typename T>
    static std::shared_ptr<MathOpMutableValue<T>> MutableValue(T x)
    {
        return std::make_shared<MathOpMutableValue<T>>(x);
    }

private:
    MathFactory() = delete;
};

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

template <typename T>
struct sine : public std::unary_function<T, T>
{
    T operator()(T x) const
    {
        return std::sin(x);
    }
};

template <typename T>
struct inverse_sine : public std::unary_function<T, T>
{
    T operator()(T x) const
    {
        return std::asin(x);
    }
};

template <typename T>
struct cosine : public std::unary_function<T, T>
{
    T operator()(T x) const
    {
        return std::cos(x);
    }
};

template <typename T>
struct inverse_cosine : public std::unary_function<T, T>
{
    T operator()(T x) const
    {
        return std::acos(x);
    }
};

template <typename T>
struct tangent : public std::unary_function<T, T>
{
    T operator()(T x) const
    {
        return std::tan(x);
    }
};

template <typename T>
struct inverse_tangent : public std::unary_function<T, T>
{
    T operator()(T x) const
    {
        return std::atan(x);
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
template<typename T> struct MathOpSqrt;
template<typename T> struct MathOpSquare;
template<typename T> struct MathOpLog;
template<typename T> struct MathOpSin;
template<typename T> struct MathOpASin;
template<typename T> struct MathOpCos;
template<typename T> struct MathOpACos;
template<typename T> struct MathOpTan;
template<typename T> struct MathOpATan;

template<typename T> std::shared_ptr<MathOp<T>> sqrt(std::shared_ptr<MathOp<T>> x) { return std::make_shared<MathOpSqrt<double>>(x); }
template<typename T> std::shared_ptr<MathOp<T>> square(std::shared_ptr<MathOp<T>> x) { return std::make_shared<MathOpSquare<double>>(x); }
template<typename T> std::shared_ptr<MathOp<T>> log(std::shared_ptr<MathOp<T>> x) { return std::make_shared<MathOpLog<double>>(x); }
template<typename T> std::shared_ptr<MathOp<T>> sin(std::shared_ptr<MathOp<T>> x) { return std::make_shared<MathOpSin<double>>(x); }
template<typename T> std::shared_ptr<MathOp<T>> asin(std::shared_ptr<MathOp<T>> x) { return std::make_shared<MathOpASin<double>>(x); }
template<typename T> std::shared_ptr<MathOp<T>> cos(std::shared_ptr<MathOp<T>> x) { return std::make_shared<MathOpCos<double>>(x); }
template<typename T> std::shared_ptr<MathOp<T>> acos(std::shared_ptr<MathOp<T>> x) { return std::make_shared<MathOpACos<double>>(x); }
template<typename T> std::shared_ptr<MathOp<T>> tan(std::shared_ptr<MathOp<T>> x) { return std::make_shared<MathOpTan<double>>(x); }
template<typename T> std::shared_ptr<MathOp<T>> atan(std::shared_ptr<MathOp<T>> x) { return std::make_shared<MathOpATan<double>>(x); }

template<typename T>
struct MathOpSqrt : public MathUnaryOp<T, square_root<T>>
{
    MathOpSqrt(std::shared_ptr<MathOp<T>>x)
        : MathUnaryOp<T, square_root<T>>(x, 2, std::make_unique<MathUnaryFunctionFormatter<T>>("√"))
    { }

    std::shared_ptr<MathOp<T>>rearranged(std::shared_ptr<MathOp<T>> for_side, std::shared_ptr<MathOp<T>>from) const override
    {
        if (for_side != this->x) return nullptr;
        return square(from);
    }
};

template<typename T>
struct MathOpSquare : public MathUnaryOp<T, squares<T>>
{
    MathOpSquare(std::shared_ptr<MathOp<T>>x)
        : MathUnaryOp<T, squares<T>>(x, 2, std::make_unique<MathUnaryPostfixFormatter<T>>("²"))
    { }

    std::shared_ptr<MathOp<T>>rearranged(std::shared_ptr<MathOp<T>> for_side, std::shared_ptr<MathOp<T>>from) const override
    {
        if (for_side != this->x) return nullptr;
        return sqrt(from);
    }
};

template<typename T>
struct MathOpLog : public MathUnaryOp<T, logarithm<T>>
{
    MathOpLog(std::shared_ptr<MathOp<T>>x)
        : MathUnaryOp<T, logarithm<T>>(x, 2, std::make_unique<MathUnaryFunctionFormatter<T>>("log"))
    { }

    std::shared_ptr<MathOp<T>>rearranged(std::shared_ptr<MathOp<T>> for_side, std::shared_ptr<MathOp<T>>from) const override
    {
        if (for_side != this->x) return nullptr;
        return MathFactory::SymbolE<T>() ^ from;
    }
};

template<typename T>
struct MathOpSin : public MathUnaryOp<T, sine<T>>
{
    MathOpSin(std::shared_ptr<MathOp<T>>x)
        : MathUnaryOp<T, sine<T>>(x, 2, std::make_unique<MathUnaryFunctionFormatter<T>>("sin"))
    { }

    std::shared_ptr<MathOp<T>>rearranged(std::shared_ptr<MathOp<T>> for_side, std::shared_ptr<MathOp<T>>from) const override
    {
        if (for_side != this->x) return nullptr;
        return asin(from);
    }
};

template<typename T>
struct MathOpASin : public MathUnaryOp<T, inverse_sine<T>>
{
    MathOpASin(std::shared_ptr<MathOp<T>>x)
        : MathUnaryOp<T, inverse_sine<T>>(x, 2, std::make_unique<MathUnaryFunctionFormatter<T>>("asin"))
    { }

    std::shared_ptr<MathOp<T>>rearranged(std::shared_ptr<MathOp<T>> for_side, std::shared_ptr<MathOp<T>>from) const override
    {
        if (for_side != this->x) return nullptr;
        return sin(from);
    }
};

template<typename T>
struct MathOpCos : public MathUnaryOp<T, cosine<T>>
{
    MathOpCos(std::shared_ptr<MathOp<T>>x)
        : MathUnaryOp<T, cosine<T>>(x, 2, std::make_unique<MathUnaryFunctionFormatter<T>>("cos"))
    { }

    std::shared_ptr<MathOp<T>>rearranged(std::shared_ptr<MathOp<T>> for_side, std::shared_ptr<MathOp<T>>from) const override
    {
        if (for_side != this->x) return nullptr;
        return acos(from);
    }
};

template<typename T>
struct MathOpACos : public MathUnaryOp<T, inverse_cosine<T>>
{
    MathOpACos(std::shared_ptr<MathOp<T>>x)
        : MathUnaryOp<T, inverse_cosine<T>>(x, 2, std::make_unique<MathUnaryFunctionFormatter<T>>("acos"))
    { }

    std::shared_ptr<MathOp<T>>rearranged(std::shared_ptr<MathOp<T>> for_side, std::shared_ptr<MathOp<T>>from) const override
    {
        if (for_side != this->x) return nullptr;
        return cos(from);
    }
};

template<typename T>
struct MathOpTan : public MathUnaryOp<T, tangent<T>>
{
    MathOpTan(std::shared_ptr<MathOp<T>>x)
        : MathUnaryOp<T, tangent<T>>(x, 2, std::make_unique<MathUnaryFunctionFormatter<T>>("tan"))
    { }

    std::shared_ptr<MathOp<T>>rearranged(std::shared_ptr<MathOp<T>> for_side, std::shared_ptr<MathOp<T>>from) const override
    {
        if (for_side != this->x) return nullptr;
        return atan(from);
    }
};

template<typename T>
struct MathOpATan : public MathUnaryOp<T, inverse_tangent<T>>
{
    MathOpATan(std::shared_ptr<MathOp<T>>x)
        : MathUnaryOp<T, inverse_tangent<T>>(x, 2, std::make_unique<MathUnaryFunctionFormatter<T>>("atan"))
    { }

    std::shared_ptr<MathOp<T>>rearranged(std::shared_ptr<MathOp<T>> for_side, std::shared_ptr<MathOp<T>>from) const override
    {
        if (for_side != this->x) return nullptr;
        return tan(from);
    }
};

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
            return from ^ (MathFactory::ConstantValue(1.0) / this->rhs);
        }
        else if (for_side == this->rhs)
        {
            return log(from) / log(this->lhs);
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
        if      (for_side == this->lhs) return from / this->rhs;
        else if (for_side == this->rhs) return from / this->lhs;
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
        if      (for_side == this->lhs) return from * this->rhs;
        else if (for_side == this->rhs) return this->lhs / from;
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
        if      (for_side == this->lhs) return from - this->rhs;
        else if (for_side == this->rhs) return from - this->lhs;
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
        if      (for_side == this->lhs) return from + this->rhs;
        else if (for_side == this->rhs) return this->lhs - from;
        else                            return nullptr;
    }
};

#endif /*ALGEBLAH_H */