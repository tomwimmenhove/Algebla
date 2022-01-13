#ifndef ALGEBLAH_H
#define ALGEBLAH_H

#include "mathfunctional.h"
#include "factory.h"
#include "visitor.h"

#include <cmath>
#include <limits>
#include <string>
#include <sstream>
#include <memory>
#include <vector>
#include <variant>

namespace MathOps
{

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

    std::shared_ptr<MathOp<T>> transform(Visitor<T>& visitor) { return std::get<std::shared_ptr<MathOp<T>>>(accept(visitor)); }
    std::shared_ptr<MathOp<T>> transform(Visitor<T>&& visitor) { return transform(visitor); }

    std::string format(Visitor<T>& visitor) { return std::get<std::string>(accept(visitor)); }
    std::string format(Visitor<T>&& visitor) { return format(visitor); }

    int count(Visitor<T>& visitor) { return std::get<int>(accept(visitor)); }
    int count(Visitor<T>&& visitor) { return format(visitor); }

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

protected:
    virtual VisitorResult<T> accept(Visitor<T>& visitor) = 0;
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
        : Value<T>(value, is_constant), symbol(symbol)
    { }

    std::string symbol;
};

template<typename T>
struct ConstantSymbol : public Symbol<T>
{
    static std::shared_ptr<ConstantSymbol<T>> create(std::string symbol, T value)
    {
        return std::shared_ptr<ConstantSymbol<T>>(new ConstantSymbol<T>(symbol, value));
    }

    void set(T x) = delete;

protected:
    VisitorResult<T> accept(Visitor<T>& visitor) override
    {
        return visitor.visit(std::static_pointer_cast<ConstantSymbol<T>>(this->shared_from_this()));
    }

private:
    ConstantSymbol(std::string symbol, T value) : Symbol<T>(symbol, value, true) { }
};

template<typename T>
struct VariableBase : public Value<T>
{
    std::string get_symbol() const { return symbol; }

protected:
    VariableBase(std::string symbol, T x)
        : Value<T>(x, false), symbol(symbol)
    { }

private:
    std::string symbol;
};

template<typename T>
struct Variable : public VariableBase<T>
{
    static std::shared_ptr<Variable<T>> create(std::string symbol, T x = 0)
    {
        return std::shared_ptr<Variable<T>>(new Variable<T>(symbol, x));
    }

protected:
    VisitorResult<T> accept(Visitor<T>& visitor) override
    {
        return visitor.visit(std::static_pointer_cast<Variable<T>>(this->shared_from_this()));
    }
    
private:
    Variable(std::string symbol, T x) : VariableBase<T>(symbol, x) { }
};

template<typename T>
struct ValueVariable : public VariableBase<T>
{
    static std::shared_ptr<ValueVariable<T>> create(std::string symbol, T x = 0)
    {
        return std::shared_ptr<ValueVariable<T>>(new ValueVariable<T>(symbol, x));
    }

protected:
    VisitorResult<T> accept(Visitor<T>& visitor) override
    {
        return visitor.visit(std::static_pointer_cast<ValueVariable<T>>(this->shared_from_this()));
    }

    ValueVariable(std::string symbol, T x) : VariableBase<T>(symbol, x) { }
};

template<typename T>
struct NamedConstant : public ValueVariable<T>
{
    static std::shared_ptr<NamedConstant<T>> create(std::string symbol, T x = 0)
    {
        return std::shared_ptr<NamedConstant<T>>(new NamedConstant<T>(symbol, x));
    }

    void set(T x) = delete;

protected:
    VisitorResult<T> accept(Visitor<T>& visitor) override
    {
        return visitor.visit(std::static_pointer_cast<NamedConstant<T>>(this->shared_from_this()));
    }

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

protected:
    VisitorResult<T> accept(Visitor<T>& visitor) override
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

    void set(T x) = delete;

protected:
    VisitorResult<T> accept(Visitor<T>& visitor) override
    {
        return visitor.visit(std::static_pointer_cast<ConstantValue<T>>(this->shared_from_this()));
    }

private:
    ConstantValue(T value) : Value<T>(value, true) { }
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

/* Unary math operations */
template<typename T>
struct Negate : public MathUnaryOp<T, negate<T>>
{
    static auto create(std::shared_ptr<MathOp<T>> x)
    {
        return std::shared_ptr<Negate<T>>(new Negate<T>(x));
    }

protected:
    VisitorResult<T> accept(Visitor<T>& visitor) override
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

protected:
    VisitorResult<T> accept(Visitor<T>& visitor) override
    {
        return visitor.visit(std::static_pointer_cast<Sqrt<T>>(this->shared_from_this()));
    }

private:
    Sqrt(std::shared_ptr<MathOp<T>> x)
        : MathUnaryOp<T, square_root<T>>(x)
    { }
};

template<typename T>
struct Log : public MathUnaryOp<T, logarithm<T>>
{
    static auto create(std::shared_ptr<MathOp<T>> x) { return std::shared_ptr<Log<T>>(new Log<T>(x)); }

protected:
    VisitorResult<T> accept(Visitor<T>& visitor) override
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

protected:
    VisitorResult<T> accept(Visitor<T>& visitor) override
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

protected:
    VisitorResult<T> accept(Visitor<T>& visitor) override
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

protected:
    VisitorResult<T> accept(Visitor<T>& visitor) override
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

protected:
    VisitorResult<T> accept(Visitor<T>& visitor) override
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

protected:
    VisitorResult<T> accept(Visitor<T>& visitor) override
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

protected:
    VisitorResult<T> accept(Visitor<T>& visitor) override
    {
        return visitor.visit(std::static_pointer_cast<ATan<T>>(this->shared_from_this()));
    }

private:
    ATan(std::shared_ptr<MathOp<T>>x)
        : MathUnaryOp<T, inverse_tangent<T>>(x)
    { }
};

template<typename T> std::shared_ptr<MathOp<T>> sqrt(std::shared_ptr<MathOp<T>> x)   { return Sqrt<T>::create(x); }
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

    bool is_commutative() const override { return false; }
    bool right_associative() const override { return true; }

protected:
    VisitorResult<T> accept(Visitor<T>& visitor) override
    {
        return visitor.visit(std::static_pointer_cast<Pow<T>>(this->shared_from_this()));
    }

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

    bool is_commutative() const override { return true; }

protected:
    VisitorResult<T> accept(Visitor<T>& visitor) override
    {
        return visitor.visit(std::static_pointer_cast<Mul<T>>(this->shared_from_this()));
    }

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

    bool is_commutative() const override { return false; }

protected:
    VisitorResult<T> accept(Visitor<T>& visitor) override
    {
        return visitor.visit(std::static_pointer_cast<Div<T>>(this->shared_from_this()));
    }

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

    bool is_commutative() const override { return true; }

protected:
    VisitorResult<T> accept(Visitor<T>& visitor) override
    {
        return visitor.visit(std::static_pointer_cast<Add<T>>(this->shared_from_this()));
    }

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

    bool is_commutative() const override { return false; }

protected:
    VisitorResult<T> accept(Visitor<T>& visitor) override
    {
        return visitor.visit(std::static_pointer_cast<Sub<T>>(this->shared_from_this()));
    }

private:
    Sub(std::shared_ptr<MathOp<T>>lhs, std::shared_ptr<MathOp<T>>rhs)
        : MathBinaryOp<T, minus<T>>(lhs, rhs, Bodmas::AdditionSubtraction)
    { }
};

} /* namespace MathOps */

#endif /*ALGEBLAH_H */
