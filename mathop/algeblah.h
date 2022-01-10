#ifndef ALGEBLAH_H
#define ALGEBLAH_H

#include <cmath>
#include <limits>
#include <string>
#include <sstream>
#include <memory>
#include <vector>

namespace MathOps
{

/* Forward declarations */
template<typename T> struct MathOp;
template<typename T> struct MutableSymbol;
template<typename T> struct ConstantSymbol;
template<typename T> struct OpVariable;
template<typename T> struct ValueVariable;
template<typename T> struct NamedConstant;
template<typename T> struct MutableValue;
template<typename T> struct ConstantValue;
template<typename T> struct Negate;
template<typename T> struct Sqrt;
template<typename T> struct Square;
template<typename T> struct Log;
template<typename T> struct Sin;
template<typename T> struct ASin;
template<typename T> struct Cos;
template<typename T> struct ACos;
template<typename T> struct Tan;
template<typename T> struct ATan;
template<typename T> struct Pow;
template<typename T> struct Mul;
template<typename T> struct Div;
template<typename T> struct Add;
template<typename T> struct Sub;

template<typename T>
struct Transformer
{
    virtual std::shared_ptr<MathOp<T>> visit(std::shared_ptr<MutableSymbol<T>> op) = 0;
    virtual std::shared_ptr<MathOp<T>> visit(std::shared_ptr<ConstantSymbol<T>> op) = 0;
    virtual std::shared_ptr<MathOp<T>> visit(std::shared_ptr<OpVariable<T>> op) = 0;
    virtual std::shared_ptr<MathOp<T>> visit(std::shared_ptr<ValueVariable<T>> op) = 0;
    virtual std::shared_ptr<MathOp<T>> visit(std::shared_ptr<NamedConstant<T>> op) = 0;
    virtual std::shared_ptr<MathOp<T>> visit(std::shared_ptr<MutableValue<T>> op) = 0;
    virtual std::shared_ptr<MathOp<T>> visit(std::shared_ptr<ConstantValue<T>> op) = 0;
    virtual std::shared_ptr<MathOp<T>> visit(std::shared_ptr<Negate<T>> op) = 0;
    virtual std::shared_ptr<MathOp<T>> visit(std::shared_ptr<Sqrt<T>> op) = 0;
    virtual std::shared_ptr<MathOp<T>> visit(std::shared_ptr<Square<T>> op) = 0;
    virtual std::shared_ptr<MathOp<T>> visit(std::shared_ptr<Log<T>> op) = 0;
    virtual std::shared_ptr<MathOp<T>> visit(std::shared_ptr<Sin<T>> op) = 0;
    virtual std::shared_ptr<MathOp<T>> visit(std::shared_ptr<ASin<T>> op) = 0;
    virtual std::shared_ptr<MathOp<T>> visit(std::shared_ptr<Cos<T>> op) = 0;
    virtual std::shared_ptr<MathOp<T>> visit(std::shared_ptr<ACos<T>> op) = 0;
    virtual std::shared_ptr<MathOp<T>> visit(std::shared_ptr<Tan<T>> op) = 0;
    virtual std::shared_ptr<MathOp<T>> visit(std::shared_ptr<ATan<T>> op) = 0;
    virtual std::shared_ptr<MathOp<T>> visit(std::shared_ptr<Pow<T>> op) = 0;
    virtual std::shared_ptr<MathOp<T>> visit(std::shared_ptr<Mul<T>> op) = 0;
    virtual std::shared_ptr<MathOp<T>> visit(std::shared_ptr<Div<T>> op) = 0;
    virtual std::shared_ptr<MathOp<T>> visit(std::shared_ptr<Add<T>> op) = 0;
    virtual std::shared_ptr<MathOp<T>> visit(std::shared_ptr<Sub<T>> op) = 0;
    virtual ~Transformer() {}
};

template<typename T>
struct Formatter
{
    virtual std::string visit(std::shared_ptr<MutableSymbol<T>> op) = 0;
    virtual std::string visit(std::shared_ptr<ConstantSymbol<T>> op) = 0;
    virtual std::string visit(std::shared_ptr<OpVariable<T>> op) = 0;
    virtual std::string visit(std::shared_ptr<ValueVariable<T>> op) = 0;
    virtual std::string visit(std::shared_ptr<NamedConstant<T>> op) = 0;
    virtual std::string visit(std::shared_ptr<MutableValue<T>> op) = 0;
    virtual std::string visit(std::shared_ptr<ConstantValue<T>> op) = 0;
    virtual std::string visit(std::shared_ptr<Negate<T>> op) = 0;
    virtual std::string visit(std::shared_ptr<Sqrt<T>> op) = 0;
    virtual std::string visit(std::shared_ptr<Square<T>> op) = 0;
    virtual std::string visit(std::shared_ptr<Log<T>> op) = 0;
    virtual std::string visit(std::shared_ptr<Sin<T>> op) = 0;
    virtual std::string visit(std::shared_ptr<ASin<T>> op) = 0;
    virtual std::string visit(std::shared_ptr<Cos<T>> op) = 0;
    virtual std::string visit(std::shared_ptr<ACos<T>> op) = 0;
    virtual std::string visit(std::shared_ptr<Tan<T>> op) = 0;
    virtual std::string visit(std::shared_ptr<ATan<T>> op) = 0;
    virtual std::string visit(std::shared_ptr<Pow<T>> op) = 0;
    virtual std::string visit(std::shared_ptr<Mul<T>> op) = 0;
    virtual std::string visit(std::shared_ptr<Div<T>> op) = 0;
    virtual std::string visit(std::shared_ptr<Add<T>> op) = 0;
    virtual std::string visit(std::shared_ptr<Sub<T>> op) = 0;
    virtual ~Formatter() {}
};

/* BODMAS */
enum class Bodmas
{
    Parentheses,
    Exponents,
    MultiplicationDivision,
    AdditionSubtraction
};

/* Math operation class base class */
template<typename T>
struct MathOp : public std::enable_shared_from_this<MathOp<T>>
{
    virtual T result() const = 0;
    virtual Bodmas precedence() const = 0;
    virtual bool is_commutative() const = 0;
    virtual bool is_constant() const = 0;
    virtual bool right_associative() const { return false; }
    virtual std::shared_ptr<MathOp<T>> transform(Transformer<T>& visitor) = 0;
    virtual std::string format(Formatter<T>& visitor) = 0;

    std::shared_ptr<MathOp<T>> transform(Transformer<T>&& visitor) { return transform(visitor); }
    std::string format(Formatter<T>&& visitor) { return format(visitor); }

    friend std::shared_ptr<MathOp<T>> operator+(std::shared_ptr<MathOp<T>> lhs, std::shared_ptr<MathOp<T>> rhs)
    {
        return Add<T>::create(lhs, rhs);
    }

    friend std::shared_ptr<MathOp<T>> operator-(std::shared_ptr<MathOp<T>> lhs, std::shared_ptr<MathOp<T>> rhs)
    {
        return Sub<T>::create(lhs, rhs);
    }

    friend std::shared_ptr<MathOp<T>> operator-(std::shared_ptr<MathOp<T>> op)
    {
        return Negate<T>::create(op);
    }

    friend std::shared_ptr<MathOp<T>> operator*(std::shared_ptr<MathOp<T>> lhs, std::shared_ptr<MathOp<T>> rhs)
    {
        return Mul<T>::create(lhs, rhs);
    }

    friend std::shared_ptr<MathOp<T>> operator/(std::shared_ptr<MathOp<T>> lhs, std::shared_ptr<MathOp<T>> rhs)
    {
        return Div<T>::create(lhs, rhs);
    }

    virtual ~MathOp() { }
};

/* Primitive math values */
template<typename T>
struct Value : public MathOp<T>
{
    T result() const override { return value; }
    void set(T x) { this->value = x; }
    Bodmas precedence() const override { return Bodmas::Parentheses; }
    bool is_commutative() const override { return true; }
    bool is_constant() const override { return is_const; }

protected:
    Value(T value, bool is_constant)
        : value(value), is_const(is_constant)
    { }

    T value;

private:
    bool is_const;
};

template<typename T>
struct Symbol : public Value<T>
{
    std::string get_symbol() const { return symbol; }

protected:
    Symbol(std::string symbol, T value, bool is_constant)
        : symbol(symbol), Value<T>(value, is_constant)
    { }

    std::string symbol;
};

template<typename T>
struct MutableSymbol : public Symbol<T>
{
    static std::shared_ptr<MutableSymbol<T>> create(std::string symbol, T value)
    {
        return std::shared_ptr<MutableSymbol<T>>(new MutableSymbol<T>>(symbol, value));
    }

    std::shared_ptr<MathOp<T>> transform(Transformer<T>& visitor) override
    {
        return visitor.visit(std::static_pointer_cast<MutableSymbol<T>>(this->shared_from_this()));
    }

    std::string format(Formatter<T>& visitor) override
    {
        return visitor.visit(std::static_pointer_cast<MutableSymbol<T>>(this->shared_from_this()));
    }

protected:
    MutableSymbol(std::string symbol, T value) : Symbol<T>(symbol, value, false) { }
};

template<typename T>
struct ConstantSymbol : public Symbol<T>
{
    static std::shared_ptr<ConstantSymbol<T>> create(std::string symbol, T value)
    {
        return std::shared_ptr<ConstantSymbol<T>>(new ConstantSymbol<T>(symbol, value));
    }

    std::shared_ptr<MathOp<T>> transform(Transformer<T>& visitor) override
    {
        return visitor.visit(std::static_pointer_cast<ConstantSymbol<T>>(this->shared_from_this()));
    }

    std::string format(Formatter<T>& visitor) override
    {
        return visitor.visit(std::static_pointer_cast<ConstantSymbol<T>>(this->shared_from_this()));
    }

    void set(T x) = delete;

private:
    ConstantSymbol(std::string symbol, T value) : Symbol<T>(symbol, value, true) { }
};

template<typename T>
struct VariableBase : public Value<T>
{
    std::string get_symbol() const { return symbol; }

protected:
    VariableBase(std::string symbol, T x)
        : symbol(symbol), Value<T>(x, false)
    { }

private:
    std::string symbol;
};

template<typename T>
struct OpVariable : public VariableBase<T>
{
    static std::shared_ptr<OpVariable<T>> create(std::string symbol, T x = 0)
    {
        return std::shared_ptr<OpVariable<T>>(new OpVariable<T>(symbol, x));
    }

    std::shared_ptr<MathOp<T>> transform(Transformer<T>& visitor) override
    {
        return visitor.visit(std::static_pointer_cast<OpVariable<T>>(this->shared_from_this()));
    }

    std::string format(Formatter<T>& visitor) override
    {
        return visitor.visit(std::static_pointer_cast<OpVariable<T>>(this->shared_from_this()));
    }
    
private:
    OpVariable(std::string symbol, T x) : VariableBase<T>(symbol, x) { }
};

template<typename T>
struct ValueVariable : public VariableBase<T>
{
    static std::shared_ptr<ValueVariable<T>> create(std::string symbol, T x = 0)
    {
        return std::shared_ptr<ValueVariable<T>>(new ValueVariable<T>(symbol, x));
    }

    std::shared_ptr<MathOp<T>> transform(Transformer<T>& visitor) override
    {
        return visitor.visit(std::static_pointer_cast<ValueVariable<T>>(this->shared_from_this()));
    }

    std::string format(Formatter<T>& visitor) override
    {
        return visitor.visit(std::static_pointer_cast<ValueVariable<T>>(this->shared_from_this()));
    }

protected:
    ValueVariable(std::string symbol, T x) : VariableBase<T>(symbol, x) { }
};

template<typename T>
struct NamedConstant : public ValueVariable<T>
{
    static std::shared_ptr<NamedConstant<T>> create(std::string symbol, T x = 0)
    {
        return std::shared_ptr<NamedConstant<T>>(new NamedConstant<T>(symbol, x));
    }

    std::shared_ptr<MathOp<T>> transform(Transformer<T>& visitor) override
    {
        return visitor.visit(std::static_pointer_cast<NamedConstant<T>>(this->shared_from_this()));
    }

    std::string format(Formatter<T>& visitor) override
    {
        return visitor.visit(std::static_pointer_cast<NamedConstant<T>>(this->shared_from_this()));
    }

    void set(T x) = delete;

private:
    NamedConstant(std::string symbol, T x) : ValueVariable<T>(symbol, x) { }
};

template<typename T>
struct MutableValue : public Value<T>
{
    static std::shared_ptr<MutableValue<T>> create(T value)
    {
        return std::make_shared<MutableValue<T>>(value);
    }

    std::shared_ptr<MathOp<T>> transform(Transformer<T>& visitor) override
    {
        return visitor.visit(std::static_pointer_cast<MutableValue<T>>(this->shared_from_this()));
    }

    std::string format(Formatter<T>& visitor) override
    {
        return visitor.visit(std::static_pointer_cast<MutableValue<T>>(this->shared_from_this()));
    }

private:
    MutableValue(T value) : Value<T>(value, false) { }
};

template<typename T>
struct ConstantValue : public Value<T>
{
    static std::shared_ptr<ConstantValue<T>> create(T value)
    {
        return std::shared_ptr<ConstantValue<T>>(new ConstantValue(value));
    }

    std::shared_ptr<MathOp<T>> transform(Transformer<T>& visitor) override
    {
        return visitor.visit(std::static_pointer_cast<ConstantValue<T>>(this->shared_from_this()));
    }

    std::string format(Formatter<T>& visitor) override
    {
        return visitor.visit(std::static_pointer_cast<ConstantValue<T>>(this->shared_from_this()));
    }

    void set(T x) = delete;

private:
    ConstantValue(T value) : Value<T>(value, true) { }
};

struct Factory
{
    template <typename T>
    static std::shared_ptr<MathOp<T>> CreateSymbolPi() { return ConstantSymbol<T>::create("%pi", HelperFunctionGetConstantPi<T>()); }

    template <typename T>
    static std::shared_ptr<MathOp<T>> CreateSymbolE() { return ConstantSymbol<T>::create("%e", HelperFunctionGetConstantE<T>()); }

    template <typename T>
    static std::shared_ptr<OpVariable<T>> CreateVariable(std::string symbol, T c = 0)
    {
        return OpVariable<T>::create(symbol, c);
    }

    template <typename T>
    static std::shared_ptr<ValueVariable<T>> CreateValueVariable(std::string symbol, T c = 0)
    {
        return ValueVariable<T>::create(symbol, c);
    }

    template <typename T>
    static std::shared_ptr<NamedConstant<T>> CreateNamedConstant(std::string symbol, T c = 0)
    {
        return NamedConstant<T>::create(symbol, c);
    }
    
    template <typename T>
    static std::shared_ptr<ConstantValue<T>> CreateConstantValue(T c)
    {
        return ConstantValue<T>::create(c);
    }

    template <typename T>
    static std::shared_ptr<MutableValue<T>> CreateMutableValue(T x)
    {
        return MutableValue<T>::create(x);
    }

private:
    Factory() = delete;
};

/* Unary math operation base class */
template<typename T, typename U>
struct MathUnaryOp : public MathOp<T>
{
    T result() const override { return f(x->result()); }
    Bodmas precedence() const override { return Bodmas::Parentheses; }
    bool is_commutative() const override { return true; }
    bool is_constant() const override { return false; }

    std::shared_ptr<MathOp<T>> get_x() const { return x; }

protected:
    MathUnaryOp(std::shared_ptr<MathOp<T>> x)
        : x(x)
    { }

    std::shared_ptr<MathOp<T>>x;
    U f;
};

/* Binary math operation base class */
template<typename T, typename U>
struct MathBinaryOp : public MathOp<T>
{
    T result() const override { return f(lhs->result(), rhs->result()); }
    Bodmas precedence() const override { return prec; }
    bool is_constant() const override { return false; }

    std::shared_ptr<MathOp<T>> get_lhs() const { return lhs; }
    std::shared_ptr<MathOp<T>> get_rhs() const { return rhs; }

protected:
    MathBinaryOp(std::shared_ptr<MathOp<T>> lhs, std::shared_ptr<MathOp<T>> rhs, Bodmas precedence)
        : lhs(lhs), rhs(rhs), prec(precedence)
    { }

    std::shared_ptr<MathOp<T>>lhs;
    std::shared_ptr<MathOp<T>>rhs;
    Bodmas prec;
    U f;
};

/* Unary operation helpers */
template <typename T>
struct negate : public std::unary_function<T, T>
{
    T operator()(T x) const
    {
        return -x;
    }
};

template <typename T>
struct logarithm : public std::unary_function<T, T>
{
    T operator()(T x) const
    {
        return HelperFunctionLog(x);
    }
};

template <typename T>
struct square_root : public std::unary_function<T, T>
{
    T operator()(T x) const
    {
        return HelperFunctionSqrt(x);
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
        return HelperFunctionSin(x);
    }
};

template <typename T>
struct inverse_sine : public std::unary_function<T, T>
{
    T operator()(T x) const
    {
        return HelperFunctionAsin(x);
    }
};

template <typename T>
struct cosine : public std::unary_function<T, T>
{
    T operator()(T x) const
    {
        return HelperFunctionCos(x);
    }
};

template <typename T>
struct inverse_cosine : public std::unary_function<T, T>
{
    T operator()(T x) const
    {
        return HelperFunctionAcos(x);
    }
};

template <typename T>
struct tangent : public std::unary_function<T, T>
{
    T operator()(T x) const
    {
        return HelperFunctionTan(x);
    }
};

template <typename T>
struct inverse_tangent : public std::unary_function<T, T>
{
    T operator()(T x) const
    {
        return HelperFunctionAtan(x);
    }
};

/* Binary operation helpers */
template <typename T>
struct raises : public std::binary_function<T, T, T>
{
    T operator()(T x, T y) const
    {
        return HelperFunctionPow(x, y);
    }
};

template <typename T>
struct multiplies : public std::binary_function<T, T, T>
{
    T operator()(T x, T y) const
    {
        return x * y;
    }
};

template <typename T>
struct divides : public std::binary_function<T, T, T>
{
    T operator()(T x, T y) const
    {
        return x / y;
    }
};

template <typename T>
struct plus : public std::binary_function<T, T, T>
{
    T operator()(T x, T y) const
    {
        return x + y;
    }
};

template <typename T>
struct minus : public std::binary_function<T, T, T>
{
    T operator()(T x, T y) const
    {
        return x - y;
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
struct Negate : public MathUnaryOp<T, negate<T>>
{
    static auto create(std::shared_ptr<MathOp<T>> x)
    {
        return std::shared_ptr<Negate<T>>(new Negate<T>(x));
    }

    std::shared_ptr<MathOp<T>> transform(Transformer<T>& visitor) override
    {
        return visitor.visit(std::static_pointer_cast<Negate<T>>(this->shared_from_this()));
    }

    std::string format(Formatter<T>& visitor) override
    {
        return visitor.visit(std::static_pointer_cast<Negate<T>>(this->shared_from_this()));
    }

private:
    Negate(std::shared_ptr<MathOp<T>> x)
        : MathUnaryOp<T, negate<T>>(x)
    { }
};

template<typename T>
struct Sqrt : public MathUnaryOp<T, square_root<T>>
{
    static auto create(std::shared_ptr<MathOp<T>> x)
    {
        return std::shared_ptr<Sqrt<T>>(new Sqrt<T>(x));
    }

    std::shared_ptr<MathOp<T>> transform(Transformer<T>& visitor) override
    {
        return visitor.visit(std::static_pointer_cast<Sqrt<T>>(this->shared_from_this()));
    }

    std::string format(Formatter<T>& visitor) override
    {
        return visitor.visit(std::static_pointer_cast<Sqrt<T>>(this->shared_from_this()));
    }

private:
    Sqrt(std::shared_ptr<MathOp<T>> x)
        : MathUnaryOp<T, square_root<T>>(x)
    { }
};

template<typename T>
struct Square : public MathUnaryOp<T, squares<T>>
{
    static auto create(std::shared_ptr<MathOp<T>> x)
    {
        return std::shared_ptr<Square<T>>(new Square<T>(x));
    }

    std::shared_ptr<MathOp<T>> transform(Transformer<T>& visitor) override
    {
        return visitor.visit(std::static_pointer_cast<Square<T>>(this->shared_from_this()));
    }

    std::string format(Formatter<T>& visitor) override
    {
        return visitor.visit(std::static_pointer_cast<Square<T>>(this->shared_from_this()));
    }

private:
    Square(std::shared_ptr<MathOp<T>> x)
        : MathUnaryOp<T, squares<T>>(x)
    { }
};

template<typename T>
struct Log : public MathUnaryOp<T, logarithm<T>>
{
    static auto create(std::shared_ptr<MathOp<T>> x) { return std::shared_ptr<Log<T>>(new Log<T>(x)); }

    std::shared_ptr<MathOp<T>> transform(Transformer<T>& visitor) override
    {
        return visitor.visit(std::static_pointer_cast<Log<T>>(this->shared_from_this()));
    }

    std::string format(Formatter<T>& visitor) override
    {
        return visitor.visit(std::static_pointer_cast<Log<T>>(this->shared_from_this()));
    }

private:
    Log(std::shared_ptr<MathOp<T>>x)
        : MathUnaryOp<T, logarithm<T>>(x)
    { }
};

template<typename T>
struct Sin : public MathUnaryOp<T, sine<T>>
{
    static auto create(std::shared_ptr<MathOp<T>> x) { return std::shared_ptr<Sin<T>>(new Sin<T>(x)); }

    std::shared_ptr<MathOp<T>> transform(Transformer<T>& visitor) override
    {
        return visitor.visit(std::static_pointer_cast<Sin<T>>(this->shared_from_this()));
    }

    std::string format(Formatter<T>& visitor) override
    {
        return visitor.visit(std::static_pointer_cast<Sin<T>>(this->shared_from_this()));
    }

private:
    Sin(std::shared_ptr<MathOp<T>>x)
        : MathUnaryOp<T, sine<T>>(x)
    { }
};

template<typename T>
struct ASin : public MathUnaryOp<T, inverse_sine<T>>
{
    static auto create(std::shared_ptr<MathOp<T>> x) { return std::shared_ptr<ASin<T>>(new ASin<T>(x)); }

    std::shared_ptr<MathOp<T>> transform(Transformer<T>& visitor) override
    {
        return visitor.visit(std::static_pointer_cast<ASin<T>>(this->shared_from_this()));
    }

    std::string format(Formatter<T>& visitor) override
    {
        return visitor.visit(std::static_pointer_cast<ASin<T>>(this->shared_from_this()));
    }

private:
    ASin(std::shared_ptr<MathOp<T>>x)
        : MathUnaryOp<T, inverse_sine<T>>(x)
    { }
};

template<typename T>
struct Cos : public MathUnaryOp<T, cosine<T>>
{
    static auto create(std::shared_ptr<MathOp<T>> x) { return std::shared_ptr<Cos<T>>(new Cos<T>(x)); }

    std::shared_ptr<MathOp<T>> transform(Transformer<T>& visitor) override
    {
        return visitor.visit(std::static_pointer_cast<Cos<T>>(this->shared_from_this()));
    }

    std::string format(Formatter<T>& visitor) override
    {
        return visitor.visit(std::static_pointer_cast<Cos<T>>(this->shared_from_this()));
    }

private:
    Cos(std::shared_ptr<MathOp<T>>x)
        : MathUnaryOp<T, cosine<T>>(x)
    { }
};

template<typename T>
struct ACos : public MathUnaryOp<T, inverse_cosine<T>>
{
    static auto create(std::shared_ptr<MathOp<T>> x) { return std::shared_ptr<ACos<T>>(new ACos<T>(x)); }

    std::shared_ptr<MathOp<T>> transform(Transformer<T>& visitor) override
    {
        return visitor.visit(std::static_pointer_cast<ACos<T>>(this->shared_from_this()));
    }

    std::string format(Formatter<T>& visitor) override
    {
        return visitor.visit(std::static_pointer_cast<ACos<T>>(this->shared_from_this()));
    }

private:
    ACos(std::shared_ptr<MathOp<T>>x)
        : MathUnaryOp<T, inverse_cosine<T>>(x)
    { }
};

template<typename T>
struct Tan : public MathUnaryOp<T, tangent<T>>
{
    static auto create(std::shared_ptr<MathOp<T>> x) { return std::shared_ptr<Tan<T>>(new Tan<T>(x)); }

    std::shared_ptr<MathOp<T>> transform(Transformer<T>& visitor) override
    {
        return visitor.visit(std::static_pointer_cast<Tan<T>>(this->shared_from_this()));
    }

    std::string format(Formatter<T>& visitor) override
    {
        return visitor.visit(std::static_pointer_cast<Tan<T>>(this->shared_from_this()));
    }

private:
    Tan(std::shared_ptr<MathOp<T>>x)
        : MathUnaryOp<T, tangent<T>>(x)
    { }
};

template<typename T>
struct ATan : public MathUnaryOp<T, inverse_tangent<T>>
{
    static auto create(std::shared_ptr<MathOp<T>> x) { return std::shared_ptr<ATan<T>>(new ATan<T>(x)); }

    std::shared_ptr<MathOp<T>> transform(Transformer<T>& visitor) override
    {
        return visitor.visit(std::static_pointer_cast<ATan<T>>(this->shared_from_this()));
    }

    std::string format(Formatter<T>& visitor) override
    {
        return visitor.visit(std::static_pointer_cast<ATan<T>>(this->shared_from_this()));
    }

private:
    ATan(std::shared_ptr<MathOp<T>>x)
        : MathUnaryOp<T, inverse_tangent<T>>(x)
    { }
};

template<typename T> std::shared_ptr<MathOp<T>> sqrt(std::shared_ptr<MathOp<T>> x)   { return Sqrt<T>::create(x); }
template<typename T> std::shared_ptr<MathOp<T>> square(std::shared_ptr<MathOp<T>> x) { return Square<T>::create(x); }
template<typename T> std::shared_ptr<MathOp<T>> log(std::shared_ptr<MathOp<T>> x)    { return Log<T>::create(x); }
template<typename T> std::shared_ptr<MathOp<T>> sin(std::shared_ptr<MathOp<T>> x)    { return Sin<T>::create(x); }
template<typename T> std::shared_ptr<MathOp<T>> asin(std::shared_ptr<MathOp<T>> x)   { return ASin<T>::create(x); }
template<typename T> std::shared_ptr<MathOp<T>> cos(std::shared_ptr<MathOp<T>> x)    { return Cos<T>::create(x); }
template<typename T> std::shared_ptr<MathOp<T>> acos(std::shared_ptr<MathOp<T>> x)   { return ACos<T>::create(x); }
template<typename T> std::shared_ptr<MathOp<T>> tan(std::shared_ptr<MathOp<T>> x)    { return Tan<T>::create(x); }
template<typename T> std::shared_ptr<MathOp<T>> atan(std::shared_ptr<MathOp<T>> x)   { return ATan<T>::create(x); }

/* Binary math operations */
template<typename T>
struct Pow : public MathBinaryOp<T, raises<T>>
{
    static auto create(std::shared_ptr<MathOp<T>>lhs, std::shared_ptr<MathOp<T>>rhs)
    {
        return std::shared_ptr<Pow<T>>(new Pow<T>(lhs, rhs));
    }

    std::shared_ptr<MathOp<T>> transform(Transformer<T>& visitor) override
    {
        return visitor.visit(std::static_pointer_cast<Pow<T>>(this->shared_from_this()));
    }

    std::string format(Formatter<T>& visitor) override
    {
        return visitor.visit(std::static_pointer_cast<Pow<T>>(this->shared_from_this()));
    }

    bool is_commutative() const override { return false; }
    bool right_associative() const override { return true; }

private:
    Pow(std::shared_ptr<MathOp<T>>lhs, std::shared_ptr<MathOp<T>>rhs)
        : MathBinaryOp<T, raises<T>>(lhs, rhs, Bodmas::Exponents)
    { }
};

template<typename T>
struct Mul : public MathBinaryOp<T, multiplies<T>>
{
    static auto create(std::shared_ptr<MathOp<T>>lhs, std::shared_ptr<MathOp<T>> rhs)
    {
        return std::shared_ptr<Mul<T>>(new Mul<T>(lhs, rhs));
    }

    std::shared_ptr<MathOp<T>> transform(Transformer<T>& visitor) override
    {
        return visitor.visit(std::static_pointer_cast<Mul<T>>(this->shared_from_this()));
    }

    std::string format(Formatter<T>& visitor) override
    {
        return visitor.visit(std::static_pointer_cast<Mul<T>>(this->shared_from_this()));
    }

    bool is_commutative() const override { return true; }

private:
    Mul(std::shared_ptr<MathOp<T>>lhs, std::shared_ptr<MathOp<T>>rhs)
        : MathBinaryOp<T, multiplies<T>>(lhs, rhs, Bodmas::MultiplicationDivision)
    { }
};

template<typename T>
struct Div : public MathBinaryOp<T, divides<T>>
{
    static auto create(std::shared_ptr<MathOp<T>>lhs, std::shared_ptr<MathOp<T>> rhs)
    {
        return std::shared_ptr<Div<T>>(new Div<T>(lhs, rhs));
    }

    std::shared_ptr<MathOp<T>> transform(Transformer<T>& visitor) override
    {
        return visitor.visit(std::static_pointer_cast<Div<T>>(this->shared_from_this()));
    }

    std::string format(Formatter<T>& visitor) override
    {
        return visitor.visit(std::static_pointer_cast<Div<T>>(this->shared_from_this()));
    }

    bool is_commutative() const override { return false; }

private:
    Div(std::shared_ptr<MathOp<T>>lhs, std::shared_ptr<MathOp<T>>rhs)
        : MathBinaryOp<T, divides<T>>(lhs, rhs, Bodmas::MultiplicationDivision)
    { }
};

template<typename T>
struct Add : public MathBinaryOp<T, plus<T>>
{
    static auto create(std::shared_ptr<MathOp<T>>lhs, std::shared_ptr<MathOp<T>>rhs)
    {
        return std::shared_ptr<Add<T>>(new Add<T>(lhs, rhs));
    }

    std::shared_ptr<MathOp<T>> transform(Transformer<T>& visitor) override
    {
        return visitor.visit(std::static_pointer_cast<Add<T>>(this->shared_from_this()));
    }

    std::string format(Formatter<T>& visitor) override
    {
        return visitor.visit(std::static_pointer_cast<Add<T>>(this->shared_from_this()));
    }

    bool is_commutative() const override { return true; }

private:
    Add(std::shared_ptr<MathOp<T>>lhs, std::shared_ptr<MathOp<T>>rhs)
        : MathBinaryOp<T, plus<T>>(lhs, rhs, Bodmas::AdditionSubtraction)
    { }
};

template<typename T>
struct Sub : public MathBinaryOp<T, minus<T>>
{
    static auto create(std::shared_ptr<MathOp<T>>lhs, std::shared_ptr<MathOp<T>>rhs)
    {
        return std::shared_ptr<Sub<T>>(new Sub<T>(lhs, rhs));
    }

    std::shared_ptr<MathOp<T>> transform(Transformer<T>& visitor) override
    {
        return visitor.visit(std::static_pointer_cast<Sub<T>>(this->shared_from_this()));
    }

    std::string format(Formatter<T>& visitor) override
    {
        return visitor.visit(std::static_pointer_cast<Sub<T>>(this->shared_from_this()));
    }

    bool is_commutative() const override { return false; }

private:
    Sub(std::shared_ptr<MathOp<T>>lhs, std::shared_ptr<MathOp<T>>rhs)
        : MathBinaryOp<T, minus<T>>(lhs, rhs, Bodmas::AdditionSubtraction)
    { }
};

} /* namespace MathOps */

#endif /*ALGEBLAH_H */
