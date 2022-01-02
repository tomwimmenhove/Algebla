#ifndef ALGEBLAH_H
#define ALGEBLAH_H

#include <cmath>
#include <limits>
#include <string>
#include <sstream>
#include <memory>
#include <vector>

/* Forward declarations */
template<typename T> struct MathOp;
template<typename T> struct MathOpMutableSymbol;
template<typename T> struct MathOpConstantSymbol;
template<typename T> struct MathOpVariable;
template<typename T> struct MathOpMutableValue;
template<typename T> struct MathOpConstantValue;
template<typename T> struct MathOpSqrt;
template<typename T> struct MathOpSquare;
template<typename T> struct MathOpLog;
template<typename T> struct MathOpSin;
template<typename T> struct MathOpASin;
template<typename T> struct MathOpCos;
template<typename T> struct MathOpACos;
template<typename T> struct MathOpTan;
template<typename T> struct MathOpATan;
template<typename T> struct MathOpPow;
template<typename T> struct MathOpMul;
template<typename T> struct MathOpDiv;
template<typename T> struct MathOpAdd;
template<typename T> struct MathOpSub;

template<typename T>
struct MathOpVisitor
{
    virtual std::shared_ptr<MathOp<T>> visit(std::shared_ptr<MathOpMutableSymbol<T>> op) = 0;
    virtual std::shared_ptr<MathOp<T>> visit(std::shared_ptr<MathOpConstantSymbol<T>> op) = 0;
    virtual std::shared_ptr<MathOp<T>> visit(std::shared_ptr<MathOpVariable<T>> op) = 0;
    virtual std::shared_ptr<MathOp<T>> visit(std::shared_ptr<MathOpMutableValue<T>> op) = 0;
    virtual std::shared_ptr<MathOp<T>> visit(std::shared_ptr<MathOpConstantValue<T>> op) = 0;
    virtual std::shared_ptr<MathOp<T>> visit(std::shared_ptr<MathOpSqrt<T>> op) = 0;
    virtual std::shared_ptr<MathOp<T>> visit(std::shared_ptr<MathOpSquare<T>> op) = 0;
    virtual std::shared_ptr<MathOp<T>> visit(std::shared_ptr<MathOpLog<T>> op) = 0;
    virtual std::shared_ptr<MathOp<T>> visit(std::shared_ptr<MathOpSin<T>> op) = 0;
    virtual std::shared_ptr<MathOp<T>> visit(std::shared_ptr<MathOpASin<T>> op) = 0;
    virtual std::shared_ptr<MathOp<T>> visit(std::shared_ptr<MathOpCos<T>> op) = 0;
    virtual std::shared_ptr<MathOp<T>> visit(std::shared_ptr<MathOpACos<T>> op) = 0;
    virtual std::shared_ptr<MathOp<T>> visit(std::shared_ptr<MathOpTan<T>> op) = 0;
    virtual std::shared_ptr<MathOp<T>> visit(std::shared_ptr<MathOpATan<T>> op) = 0;
    virtual std::shared_ptr<MathOp<T>> visit(std::shared_ptr<MathOpPow<T>> op) = 0;
    virtual std::shared_ptr<MathOp<T>> visit(std::shared_ptr<MathOpMul<T>> op) = 0;
    virtual std::shared_ptr<MathOp<T>> visit(std::shared_ptr<MathOpDiv<T>> op) = 0;
    virtual std::shared_ptr<MathOp<T>> visit(std::shared_ptr<MathOpAdd<T>> op) = 0;
    virtual std::shared_ptr<MathOp<T>> visit(std::shared_ptr<MathOpSub<T>> op) = 0;
    virtual ~MathOpVisitor() {}
};
#include <iostream>
/* Math operation class base class */
template<typename T>
struct MathOp : public std::enable_shared_from_this<MathOp<T>>
{
    virtual T result() const = 0;
    virtual int precedence() const = 0;
    virtual bool is_commutative() const = 0;
    virtual bool is_constant() const = 0;
    virtual std::shared_ptr<MathOp<T>> rearranged(
        std::shared_ptr<MathOp<T>> for_side, std::shared_ptr<MathOp<T>> from) const = 0;
    virtual std::shared_ptr<MathOp<T>> accept(MathOpVisitor<T>& visitor) = 0;

    std::shared_ptr<MathOp<T>> accept(MathOpVisitor<T>&& visitor) { return accept(visitor); }

    std::string parenthesize(int parent_precedence, bool parent_is_commutative, bool use_commutation) const
    {
        std::stringstream ss;

        bool use_parens = !use_commutation || parent_is_commutative
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
        return stream << op.parenthesize(std::numeric_limits<int>::max(), true, false);
    }

    friend std::shared_ptr<MathOp<T>> operator+(std::shared_ptr<MathOp<T>> lhs, std::shared_ptr<MathOp<T>> rhs)
    {
        return MathOpAdd<T>::create(lhs, rhs);
    }

    friend std::shared_ptr<MathOp<T>> operator-(std::shared_ptr<MathOp<T>> lhs, std::shared_ptr<MathOp<T>> rhs)
    {
        return MathOpSub<T>::create(lhs, rhs);
    }

    friend std::shared_ptr<MathOp<T>> operator*(std::shared_ptr<MathOp<T>> lhs, std::shared_ptr<MathOp<T>> rhs)
    {
        return MathOpMul<T>::create(lhs, rhs);
    }

    friend std::shared_ptr<MathOp<T>> operator/(std::shared_ptr<MathOp<T>> lhs, std::shared_ptr<MathOp<T>> rhs)
    {
        return MathOpDiv<T>::create(lhs, rhs);
    }

    friend std::shared_ptr<MathOp<T>> operator^(std::shared_ptr<MathOp<T>> lhs, std::shared_ptr<MathOp<T>> rhs)
    {
        return MathOpPow<T>::create(lhs, rhs);
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
        int is_commutative = op.is_commutative();
        return stream << lhs->parenthesize(precedence, is_commutative, false)
                      << symbol
                      << rhs->parenthesize(precedence, is_commutative, true);
    }

private:
    std::string symbol;
};


/* Primitive math values */
template<typename T>
struct MathOpValue : public MathOp<T>
{
    T result() const override { return value; }
    void set(T x) { this->value = x; }
    int precedence() const override { return 0; }
    bool is_commutative() const override { return true; }
    bool is_constant() const override { return is_const; }
    std::shared_ptr<MathOp<T>> rearranged(std::shared_ptr<MathOp<T>>, std::shared_ptr<MathOp<T>>) const override { return nullptr; }
    std::ostream& to_stream(std::ostream& stream) const { return stream << symbol_str(); }

protected:
    MathOpValue(T value, bool is_constant)
        : value(value), is_const(is_constant)
    { }

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
protected:
    MathOpSymbol(std::string symbol, T value, bool is_constant)
        : symbol(symbol), MathOpValue<T>(value, is_constant)
    { }

    std::string symbol_str() const override { return symbol; }
    std::string symbol;
};

template<typename T>
struct MathOpMutableSymbol : public MathOpSymbol<T>
{
    static std::shared_ptr<MathOpMutableSymbol<T>> create(std::string symbol, T value)
    {
        return std::shared_ptr<MathOpMutableSymbol<T>>(new MathOpMutableSymbol<T>>(symbol, value));
    }

    std::shared_ptr<MathOp<T>> accept(MathOpVisitor<T>& visitor) override
    {
        return visitor.visit(std::static_pointer_cast<MathOpMutableSymbol<T>>(this->shared_from_this()));
    }

protected:
    MathOpMutableSymbol(std::string symbol, T value) : MathOpSymbol<T>(symbol, value, false) { }
};

template<typename T>
struct MathOpConstantSymbol : public MathOpSymbol<T>
{
    static std::shared_ptr<MathOpConstantSymbol<T>> create(std::string symbol, T value)
    {
        return std::shared_ptr<MathOpConstantSymbol<T>>(new MathOpConstantSymbol<T>(symbol, value));
    }

    std::shared_ptr<MathOp<T>> accept(MathOpVisitor<T>& visitor) override
    {
        return visitor.visit(std::static_pointer_cast<MathOpConstantSymbol<T>>(this->shared_from_this()));
    }

    void set(T x) = delete;

private:
    MathOpConstantSymbol(std::string symbol, T value) : MathOpSymbol<T>(symbol, value, true) { }
};

template<typename T>
struct MathOpVariable : public MathOpValue<T>
{
    static std::shared_ptr<MathOpVariable<T>> create(std::string symbol, T x, bool show_value)
    {
        return std::shared_ptr<MathOpVariable<T>>(new MathOpVariable<T>(symbol, x, show_value));
    }

    std::shared_ptr<MathOp<T>> accept(MathOpVisitor<T>& visitor) override
    {
        return visitor.visit(std::static_pointer_cast<MathOpVariable<T>>(this->shared_from_this()));
    }

    std::string get_symbol() const { return symbol; }
    bool get_show_value() const { return show_value; }

protected:
    std::string symbol_str() const override { return show_value ? MathOpValue<T>::symbol_str() : symbol; }

private:
    MathOpVariable(std::string symbol, T x, bool show_value)
        : symbol(symbol), show_value(show_value), MathOpValue<T>(x, false)
    { }

    std::string symbol;
    bool show_value;
};

template<typename T>
struct MathOpMutableValue : public MathOpValue<T>
{
    static std::shared_ptr<MathOpMutableValue<T>> create(T value)
    {
        return std::make_shared<MathOpMutableValue<T>>(value);
    }

    std::shared_ptr<MathOp<T>> accept(MathOpVisitor<T>& visitor) override
    {
        return visitor.visit(std::static_pointer_cast<MathOpMutableValue<T>>(this->shared_from_this()));
    }

private:
    MathOpMutableValue(T value) : MathOpValue<T>(value, false) { }
};

template<typename T>
struct MathOpConstantValue : public MathOpValue<T>
{
    static std::shared_ptr<MathOpConstantValue<T>> create(T value)
    {
        return std::shared_ptr<MathOpConstantValue<T>>(new MathOpConstantValue(value));
    }

    std::shared_ptr<MathOp<T>> accept(MathOpVisitor<T>& visitor) override
    {
        return visitor.visit(std::static_pointer_cast<MathOpConstantValue<T>>(this->shared_from_this()));
    }

    void set(T x) = delete;

private:
    MathOpConstantValue(T value) : MathOpValue<T>(value, true) { }
};

struct MathFactory
{
    template <typename T>
    static std::shared_ptr<MathOp<T>> SymbolPi() { return MathOpConstantSymbol<T>::create("π", M_PI); }

    template <typename T>
    static std::shared_ptr<MathOp<T>> SymbolE() { return MathOpConstantSymbol<T>::create("e", M_E); }

    template <typename T>
    static std::shared_ptr<MathOp<T>> SymbolSqrt2() { return MathOpConstantSymbol<T>::create("√(2)", M_SQRT2); }

    template <typename T>
    static std::shared_ptr<MathOpVariable<T>> Variable(std::string symbol, T c = 0)
    {
        return MathOpVariable<T>::create(symbol, c, false);
    }

    template <typename T>
    static std::shared_ptr<MathOpVariable<T>> ValueVariable(std::string symbol, T c = 0)
    {
        return MathOpVariable<T>::create(symbol, c, true);
    }

    template <typename T>
    static std::shared_ptr<MathOpConstantValue<T>> ConstantValue(T c)
    {
        return MathOpConstantValue<T>::create(c);
    }

    template <typename T>
    static std::shared_ptr<MathOpMutableValue<T>> MutableValue(T x)
    {
        return MathOpMutableValue<T>::create(x);
    }

private:
    MathFactory() = delete;
};

/* Unary math operation base class */
template<typename T, typename U>
struct MathUnaryOp : public MathOp<T>
{
    T result() const override { return f(x->result()); }
    int precedence() const override { return prec; }
    bool is_commutative() const override { return true; }
    bool is_constant() const override { return false; }

    std::shared_ptr<MathOp<T>> get_x() const { return x; }
    int get_precedence() const { return prec; }
    std::unique_ptr<MathUnaryFormatter<T>> get_formatter() const { return formatter; }
    
    std::ostream& to_stream(std::ostream& stream) const override
    {
        return formatter->to_stream(stream, *this, x);
    }

protected:
    MathUnaryOp(std::shared_ptr<MathOp<T>> x, int precedence, std::unique_ptr<MathUnaryFormatter<T>> formatter)
        : x(x), prec(precedence), formatter(std::move(formatter))
    { }

    std::shared_ptr<MathOp<T>>x;
    int prec;
    std::unique_ptr<MathUnaryFormatter<T>> formatter;
    U f;
};

/* Binary math operation base class */
template<typename T, typename U>
struct MathBinaryOp : public MathOp<T>
{
    T result() const override { return f(lhs->result(), rhs->result()); }
    int precedence() const override { return prec; }
    bool is_constant() const override { return false; }

    std::shared_ptr<MathOp<T>> get_lhs() const { return lhs; }
    std::shared_ptr<MathOp<T>> get_rhs() const { return rhs; }
    int get_precedence() const { return prec; }
    std::unique_ptr<MathUnaryFormatter<T>> get_formatter() const { return formatter; }
    
    std::ostream& to_stream(std::ostream& stream) const override
    {
        return formatter->to_stream(stream, *this, lhs, rhs);
    }

protected:
    MathBinaryOp(std::shared_ptr<MathOp<T>>lhs, std::shared_ptr<MathOp<T>>rhs, int precedence,
        std::unique_ptr<MathBinaryFormatter<T>> formatter)
        : lhs(lhs), rhs(rhs), prec(precedence), formatter(std::move(formatter))
    { }

    std::shared_ptr<MathOp<T>>lhs;
    std::shared_ptr<MathOp<T>>rhs;
    int prec;
    std::unique_ptr<MathBinaryFormatter<T>> formatter;
    U f;
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
template<typename T> std::shared_ptr<MathOp<T>> sqrt(std::shared_ptr<MathOp<T>> x);
template<typename T> std::shared_ptr<MathOp<T>> square(std::shared_ptr<MathOp<T>> x);
template<typename T> std::shared_ptr<MathOp<T>> log(std::shared_ptr<MathOp<T>> x);
template<typename T> std::shared_ptr<MathOp<T>> sin(std::shared_ptr<MathOp<T>> x);
template<typename T> std::shared_ptr<MathOp<T>> asin(std::shared_ptr<MathOp<T>> x);
template<typename T> std::shared_ptr<MathOp<T>> cos(std::shared_ptr<MathOp<T>> x);
template<typename T> std::shared_ptr<MathOp<T>> acos(std::shared_ptr<MathOp<T>> x);
template<typename T> std::shared_ptr<MathOp<T>> tan(std::shared_ptr<MathOp<T>> x);
template<typename T> std::shared_ptr<MathOp<T>> atan(std::shared_ptr<MathOp<T>> x);

template<typename T>
struct MathOpSqrt : public MathUnaryOp<T, square_root<T>>
{
    static auto create(std::shared_ptr<MathOp<T>> x)
    {
        return std::shared_ptr<MathOpSqrt<T>>(new MathOpSqrt<T>(x));
    }

    std::shared_ptr<MathOp<T>> accept(MathOpVisitor<T>& visitor) override
    {
        return visitor.visit(std::static_pointer_cast<MathOpSqrt<T>>(this->shared_from_this()));
    }

    std::shared_ptr<MathOp<T>>rearranged(std::shared_ptr<MathOp<T>> for_side, std::shared_ptr<MathOp<T>>from) const override
    {
        if (for_side != this->x) return nullptr;
        return square(from);
    }

private:
    MathOpSqrt(std::shared_ptr<MathOp<T>> x)
        : MathUnaryOp<T, square_root<T>>(x, 2, std::make_unique<MathUnaryFunctionFormatter<T>>("√"))
    { }
};

template<typename T>
struct MathOpSquare : public MathUnaryOp<T, squares<T>>
{
    static auto create(std::shared_ptr<MathOp<T>> x)
    {
        return std::shared_ptr<MathOpSquare<T>>(new MathOpSquare<T>(x));
    }

    std::shared_ptr<MathOp<T>> accept(MathOpVisitor<T>& visitor) override
    {
        return visitor.visit(std::static_pointer_cast<MathOpSquare<T>>(this->shared_from_this()));
    }

    std::shared_ptr<MathOp<T>>rearranged(std::shared_ptr<MathOp<T>> for_side, std::shared_ptr<MathOp<T>>from) const override
    {
        if (for_side != this->x) return nullptr;
        return sqrt(from);
    }

private:
    MathOpSquare(std::shared_ptr<MathOp<T>> x)
        : MathUnaryOp<T, squares<T>>(x, 2, std::make_unique<MathUnaryPostfixFormatter<T>>("²"))
    { }
};

template<typename T>
struct MathOpLog : public MathUnaryOp<T, logarithm<T>>
{
    static auto create(std::shared_ptr<MathOp<T>> x) { return std::shared_ptr<MathOpLog<T>>(new MathOpLog<T>(x)); }

    std::shared_ptr<MathOp<T>>rearranged(std::shared_ptr<MathOp<T>> for_side, std::shared_ptr<MathOp<T>>from) const override
    {
        if (for_side != this->x) return nullptr;
        return MathFactory::SymbolE<T>() ^ from;
    }

    std::shared_ptr<MathOp<T>> accept(MathOpVisitor<T>& visitor) override
    {
        return visitor.visit(std::static_pointer_cast<MathOpLog<T>>(this->shared_from_this()));
    }

private:
    MathOpLog(std::shared_ptr<MathOp<T>>x)
        : MathUnaryOp<T, logarithm<T>>(x, 2, std::make_unique<MathUnaryFunctionFormatter<T>>("log"))
    { }
};

template<typename T>
struct MathOpSin : public MathUnaryOp<T, sine<T>>
{
    static auto create(std::shared_ptr<MathOp<T>> x) { return std::shared_ptr<MathOpSin<T>>(new MathOpSin<T>(x)); }

    std::shared_ptr<MathOp<T>>rearranged(std::shared_ptr<MathOp<T>> for_side, std::shared_ptr<MathOp<T>>from) const override
    {
        if (for_side != this->x) return nullptr;
        return asin(from);
    }

    std::shared_ptr<MathOp<T>> accept(MathOpVisitor<T>& visitor) override
    {
        return visitor.visit(std::static_pointer_cast<MathOpSin<T>>(this->shared_from_this()));
    }

private:
    MathOpSin(std::shared_ptr<MathOp<T>>x)
        : MathUnaryOp<T, sine<T>>(x, 2, std::make_unique<MathUnaryFunctionFormatter<T>>("sin"))
    { }
};

template<typename T>
struct MathOpASin : public MathUnaryOp<T, inverse_sine<T>>
{
    static auto create(std::shared_ptr<MathOp<T>> x) { return std::shared_ptr<MathOpASin<T>>(new MathOpASin<T>(x)); }

    std::shared_ptr<MathOp<T>>rearranged(std::shared_ptr<MathOp<T>> for_side, std::shared_ptr<MathOp<T>>from) const override
    {
        if (for_side != this->x) return nullptr;
        return sin(from);
    }

    std::shared_ptr<MathOp<T>> accept(MathOpVisitor<T>& visitor) override
    {
        return visitor.visit(std::static_pointer_cast<MathOpASin<T>>(this->shared_from_this()));
    }

private:
    MathOpASin(std::shared_ptr<MathOp<T>>x)
        : MathUnaryOp<T, inverse_sine<T>>(x, 2, std::make_unique<MathUnaryFunctionFormatter<T>>("asin"))
    { }
};

template<typename T>
struct MathOpCos : public MathUnaryOp<T, cosine<T>>
{
    static auto create(std::shared_ptr<MathOp<T>> x) { return std::shared_ptr<MathOpCos<T>>(new MathOpCos<T>(x)); }

    std::shared_ptr<MathOp<T>>rearranged(std::shared_ptr<MathOp<T>> for_side, std::shared_ptr<MathOp<T>>from) const override
    {
        if (for_side != this->x) return nullptr;
        return acos(from);
    }

    std::shared_ptr<MathOp<T>> accept(MathOpVisitor<T>& visitor) override
    {
        return visitor.visit(std::static_pointer_cast<MathOpCos<T>>(this->shared_from_this()));
    }

private:
    MathOpCos(std::shared_ptr<MathOp<T>>x)
        : MathUnaryOp<T, cosine<T>>(x, 2, std::make_unique<MathUnaryFunctionFormatter<T>>("cos"))
    { }
};

template<typename T>
struct MathOpACos : public MathUnaryOp<T, inverse_cosine<T>>
{
    static auto create(std::shared_ptr<MathOp<T>> x) { return std::shared_ptr<MathOpACos<T>>(new MathOpACos<T>(x)); }

    std::shared_ptr<MathOp<T>>rearranged(std::shared_ptr<MathOp<T>> for_side, std::shared_ptr<MathOp<T>>from) const override
    {
        if (for_side != this->x) return nullptr;
        return cos(from);
    }

    std::shared_ptr<MathOp<T>> accept(MathOpVisitor<T>& visitor) override
    {
        return visitor.visit(std::static_pointer_cast<MathOpACos<T>>(this->shared_from_this()));
    }

private:
    MathOpACos(std::shared_ptr<MathOp<T>>x)
        : MathUnaryOp<T, inverse_cosine<T>>(x, 2, std::make_unique<MathUnaryFunctionFormatter<T>>("acos"))
    { }
};

template<typename T>
struct MathOpTan : public MathUnaryOp<T, tangent<T>>
{
    static auto create(std::shared_ptr<MathOp<T>> x) { return std::shared_ptr<MathOpTan<T>>(new MathOpTan<T>(x)); }

    std::shared_ptr<MathOp<T>>rearranged(std::shared_ptr<MathOp<T>> for_side, std::shared_ptr<MathOp<T>>from) const override
    {
        if (for_side != this->x) return nullptr;
        return atan(from);
    }

    std::shared_ptr<MathOp<T>> accept(MathOpVisitor<T>& visitor) override
    {
        return visitor.visit(std::static_pointer_cast<MathOpTan<T>>(this->shared_from_this()));
    }

private:
    MathOpTan(std::shared_ptr<MathOp<T>>x)
        : MathUnaryOp<T, tangent<T>>(x, 2, std::make_unique<MathUnaryFunctionFormatter<T>>("tan"))
    { }
};

template<typename T>
struct MathOpATan : public MathUnaryOp<T, inverse_tangent<T>>
{
    static auto create(std::shared_ptr<MathOp<T>> x) { return std::shared_ptr<MathOpATan<T>>(new MathOpATan<T>(x)); }

    std::shared_ptr<MathOp<T>>rearranged(std::shared_ptr<MathOp<T>> for_side, std::shared_ptr<MathOp<T>>from) const override
    {
        if (for_side != this->x) return nullptr;
        return tan(from);
    }

    std::shared_ptr<MathOp<T>> accept(MathOpVisitor<T>& visitor) override
    {
        return visitor.visit(std::static_pointer_cast<MathOpATan<T>>(this->shared_from_this()));
    }

private:
    MathOpATan(std::shared_ptr<MathOp<T>>x)
        : MathUnaryOp<T, inverse_tangent<T>>(x, 2, std::make_unique<MathUnaryFunctionFormatter<T>>("atan"))
    { }
};

template<typename T> std::shared_ptr<MathOp<T>> sqrt(std::shared_ptr<MathOp<T>> x)   { return MathOpSqrt<T>::create(x); }
template<typename T> std::shared_ptr<MathOp<T>> square(std::shared_ptr<MathOp<T>> x) { return MathOpSquare<T>::create(x); }
template<typename T> std::shared_ptr<MathOp<T>> log(std::shared_ptr<MathOp<T>> x)    { return MathOpLog<T>::create(x); }
template<typename T> std::shared_ptr<MathOp<T>> sin(std::shared_ptr<MathOp<T>> x)    { return MathOpSin<T>::create(x); }
template<typename T> std::shared_ptr<MathOp<T>> asin(std::shared_ptr<MathOp<T>> x)   { return MathOpASin<T>::create(x); }
template<typename T> std::shared_ptr<MathOp<T>> cos(std::shared_ptr<MathOp<T>> x)    { return MathOpCos<T>::create(x); }
template<typename T> std::shared_ptr<MathOp<T>> acos(std::shared_ptr<MathOp<T>> x)   { return MathOpACos<T>::create(x); }
template<typename T> std::shared_ptr<MathOp<T>> tan(std::shared_ptr<MathOp<T>> x)    { return MathOpTan<T>::create(x); }
template<typename T> std::shared_ptr<MathOp<T>> atan(std::shared_ptr<MathOp<T>> x)   { return MathOpATan<T>::create(x); }

/* Binary math operations */
template<typename T>
struct MathOpPow : public MathBinaryOp<T, raises<T>>
{
    static auto create(std::shared_ptr<MathOp<T>>lhs, std::shared_ptr<MathOp<T>>rhs)
    {
        return std::shared_ptr<MathOpPow<T>>(new MathOpPow<T>(lhs, rhs));
    }

    std::shared_ptr<MathOp<T>> accept(MathOpVisitor<T>& visitor) override
    {
        return visitor.visit(std::static_pointer_cast<MathOpPow<T>>(this->shared_from_this()));
    }

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

private:
    MathOpPow(std::shared_ptr<MathOp<T>>lhs, std::shared_ptr<MathOp<T>>rhs)
        : MathBinaryOp<T, raises<T>>(lhs, rhs, 2, std::make_unique<MathBinaryOperationFormatter<T>>(" ^ "))
    { }
};

template<typename T>
struct MathOpMul : public MathBinaryOp<T, std::multiplies<T>>
{
    static auto create(std::shared_ptr<MathOp<T>>lhs, std::shared_ptr<MathOp<T>> rhs)
    {
        return std::shared_ptr<MathOpMul<T>>(new MathOpMul<T>(lhs, rhs));
    }

    std::shared_ptr<MathOp<T>> accept(MathOpVisitor<T>& visitor) override
    {
        return visitor.visit(std::static_pointer_cast<MathOpMul<T>>(this->shared_from_this()));
    }

    bool is_commutative() const override { return true; }

    std::shared_ptr<MathOp<T>>rearranged(std::shared_ptr<MathOp<T>> for_side, std::shared_ptr<MathOp<T>>from) const override
    {
        if      (for_side == this->lhs) return from / this->rhs;
        else if (for_side == this->rhs) return from / this->lhs;
        else                            return nullptr;
    }

private:
    MathOpMul(std::shared_ptr<MathOp<T>>lhs, std::shared_ptr<MathOp<T>>rhs)
        : MathBinaryOp<T, std::multiplies<T>>(lhs, rhs, 10, std::make_unique<MathBinaryOperationFormatter<T>>(" * "))
    { }
};

template<typename T>
struct MathOpDiv : public MathBinaryOp<T, std::divides<T>>
{
    static auto create(std::shared_ptr<MathOp<T>>lhs, std::shared_ptr<MathOp<T>> rhs)
    {
        return std::shared_ptr<MathOpDiv<T>>(new MathOpDiv<T>(lhs, rhs));
    }

    std::shared_ptr<MathOp<T>> accept(MathOpVisitor<T>& visitor) override
    {
        return visitor.visit(std::static_pointer_cast<MathOpDiv<T>>(this->shared_from_this()));
    }

    bool is_commutative() const override { return false; }

    std::shared_ptr<MathOp<T>>rearranged(std::shared_ptr<MathOp<T>> for_side, std::shared_ptr<MathOp<T>>from) const override
    {
        if      (for_side == this->lhs) return from * this->rhs;
        else if (for_side == this->rhs) return this->lhs / from;
        else                            return nullptr;
    }

private:
    MathOpDiv(std::shared_ptr<MathOp<T>>lhs, std::shared_ptr<MathOp<T>>rhs)
        : MathBinaryOp<T, std::divides<T>>(lhs, rhs, 10, std::make_unique<MathBinaryOperationFormatter<T>>(" / "))
    { }
};

template<typename T>
struct MathOpAdd : public MathBinaryOp<T, std::plus<T>>
{
    static auto create(std::shared_ptr<MathOp<T>>lhs, std::shared_ptr<MathOp<T>>rhs)
    {
        return std::shared_ptr<MathOpAdd<T>>(new MathOpAdd<T>(lhs, rhs));
    }

    std::shared_ptr<MathOp<T>> accept(MathOpVisitor<T>& visitor) override
    {
        return visitor.visit(std::static_pointer_cast<MathOpAdd<T>>(this->shared_from_this()));
    }

    bool is_commutative() const override { return true; }

    std::shared_ptr<MathOp<T>>rearranged(std::shared_ptr<MathOp<T>> for_side, std::shared_ptr<MathOp<T>>from) const override
    {
        if      (for_side == this->lhs) return from - this->rhs;
        else if (for_side == this->rhs) return from - this->lhs;
        else                            return nullptr;
    }

private:
    MathOpAdd(std::shared_ptr<MathOp<T>>lhs, std::shared_ptr<MathOp<T>>rhs)
        : MathBinaryOp<T, std::plus<T>>(lhs, rhs, 100, std::make_unique<MathBinaryOperationFormatter<T>>(" + "))
    { }
};

template<typename T>
struct MathOpSub : public MathBinaryOp<T, std::minus<T>>
{
    static auto create(std::shared_ptr<MathOp<T>>lhs, std::shared_ptr<MathOp<T>>rhs)
    {
        return std::shared_ptr<MathOpSub<T>>(new MathOpSub<T>(lhs, rhs));
    }

    std::shared_ptr<MathOp<T>> accept(MathOpVisitor<T>& visitor) override
    {
        return visitor.visit(std::static_pointer_cast<MathOpSub<T>>(this->shared_from_this()));
    }

    bool is_commutative() const override { return false; }

    std::shared_ptr<MathOp<T>>rearranged(std::shared_ptr<MathOp<T>> for_side, std::shared_ptr<MathOp<T>>from) const override
    {
        if      (for_side == this->lhs) return from + this->rhs;
        else if (for_side == this->rhs) return this->lhs - from;
        else                            return nullptr;
    }

private:
    MathOpSub(std::shared_ptr<MathOp<T>>lhs, std::shared_ptr<MathOp<T>>rhs)
        : MathBinaryOp<T, std::minus<T>>(lhs, rhs, 100, std::make_unique<MathBinaryOperationFormatter<T>>(" - "))
    { }
};

#endif /*ALGEBLAH_H */
