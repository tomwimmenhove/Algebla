#ifndef ALGEBLAH_H
#define ALGEBLAH_H

#include "factory.h"
#include "visitor.h"

#include <cmath>
#include <limits>
#include <string>
#include <sstream>
#include <memory>
#include <vector>
#include <variant>
#include <cassert>
#include <functional>

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
    virtual bool is_single() const = 0;
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
    bool is_single() const override { return true; }
    virtual void set(T x) { std::cerr << "Attempt to set a read-only value\n"; abort(); }
    virtual std::string get_symbol() const { std::cerr << "Attempt to get symbol name from an unnamed value\n"; abort(); }
    Bodmas precedence() const override { return Bodmas::Parentheses; }
    bool is_commutative() const override { return true; }

protected:
    Value(T value)
        : value(value)
    { }

    T value;
};

template<typename T>
struct ConstantSymbol : public Value<T>
{
    static std::shared_ptr<ConstantSymbol<T>> create(std::string symbol, T value)
    {
        return std::shared_ptr<ConstantSymbol<T>>(new ConstantSymbol<T>(symbol, value));
    }

    bool is_constant() const override { return true; }
    std::string get_symbol() const override { return symbol; }

protected:
    VisitorResult<T> accept(Visitor<T>& visitor) override
    {
        return visitor.visit(std::static_pointer_cast<ConstantSymbol<T>>(this->shared_from_this()));
    }

private:
    std::string symbol;

    ConstantSymbol(std::string symbol, T value) : Value<T>(value), symbol(symbol) { }
};

template<typename T>
struct Variable : public Value<T>
{
    static std::shared_ptr<Variable<T>> create(std::string symbol, T x = 0)
    {
        return std::shared_ptr<Variable<T>>(new Variable<T>(symbol, x));
    }

    void set(T x) override { this->value = x; };
    bool is_constant() const override { return false; }
    std::string get_symbol() const override { return symbol; }

protected:
    VisitorResult<T> accept(Visitor<T>& visitor) override
    {
        return visitor.visit(std::static_pointer_cast<Variable<T>>(this->shared_from_this()));
    }
    
private:
    std::string symbol;

    Variable(std::string symbol, T x) : Value<T>(x), symbol(symbol) { }
};

template<typename T>
struct ValueVariable : public Value<T>
{
    static std::shared_ptr<ValueVariable<T>> create(std::string symbol, T x = 0)
    {
        return std::shared_ptr<ValueVariable<T>>(new ValueVariable<T>(symbol, x));
    }

    void set(T x) override { this->value = x; };
    bool is_constant() const override { return false; }
    std::string get_symbol() const override { return symbol; }

protected:
    VisitorResult<T> accept(Visitor<T>& visitor) override
    {
        return visitor.visit(std::static_pointer_cast<ValueVariable<T>>(this->shared_from_this()));
    }

    ValueVariable(std::string symbol, T x) : Value<T>(x), symbol(symbol) { }

private:
    std::string symbol;
};

template<typename T>
struct NamedConstant : public Value<T>
{
    static std::shared_ptr<NamedConstant<T>> create(std::string symbol, T x = 0)
    {
        return std::shared_ptr<NamedConstant<T>>(new NamedConstant<T>(symbol, x));
    }

    bool is_constant() const override { return true; }
    std::string get_symbol() const override { return symbol; }

protected:
    VisitorResult<T> accept(Visitor<T>& visitor) override
    {
        return visitor.visit(std::static_pointer_cast<NamedConstant<T>>(this->shared_from_this()));
    }

private:
    std::string symbol;

    NamedConstant(std::string symbol, T x) : Value<T>(x), symbol(symbol) { }
};

template<typename T>
struct MutableValue : public Value<T>
{
    static std::shared_ptr<MutableValue<T>> create(T value)
    {
        return std::make_shared<MutableValue<T>>(value);
    }

    void set(T x) override { this->value = x; };
    bool is_constant() const override { return false; }

protected:
    VisitorResult<T> accept(Visitor<T>& visitor) override
    {
        return visitor.visit(std::static_pointer_cast<MutableValue<T>>(this->shared_from_this()));
    }

private:
    MutableValue(T value) : Value<T>(value) { }
};

template<typename T>
struct ConstantValue : public Value<T>
{
    static std::shared_ptr<ConstantValue<T>> create(T value)
    {
        return std::shared_ptr<ConstantValue<T>>(new ConstantValue(value));
    }

    bool is_constant() const override { return true; }

protected:
    VisitorResult<T> accept(Visitor<T>& visitor) override
    {
        return visitor.visit(std::static_pointer_cast<ConstantValue<T>>(this->shared_from_this()));
    }

private:
    ConstantValue(T value) : Value<T>(value) { }
};

/* Unary math operation base class */
template<typename T>
struct MathUnaryOp : public MathOp<T>
{
    Bodmas precedence() const override { return Bodmas::Parentheses; }
    bool is_single() const override { return true; }
    bool is_commutative() const override { return true; }
    bool is_constant() const override { return false; }

    std::shared_ptr<MathOp<T>> get_x() const { return x; }

protected:
    MathUnaryOp(std::shared_ptr<MathOp<T>> x)
        : x(x)
    { }

    std::shared_ptr<MathOp<T>>x;
};

/* Binary math operation base class */
template<typename T>
struct MathBinaryOp : public MathOp<T>
{
    Bodmas precedence() const override { return prec; }
    bool is_constant() const override { return false; }
    bool is_single() const override { return false; }

    std::shared_ptr<MathOp<T>> get_lhs() const { return lhs; }
    std::shared_ptr<MathOp<T>> get_rhs() const { return rhs; }

protected:
    MathBinaryOp(std::shared_ptr<MathOp<T>> lhs, std::shared_ptr<MathOp<T>> rhs, Bodmas precedence)
        : lhs(lhs), rhs(rhs), prec(precedence)
    { }

    std::shared_ptr<MathOp<T>>lhs;
    std::shared_ptr<MathOp<T>>rhs;
    Bodmas prec;
};

/* Unary math operations */
#define DEFINE_UNARY_OP(op_name, operation)                                                     \
template<typename T>                                                                            \
struct op_name : public MathUnaryOp<T>                                                          \
{                                                                                               \
    T result() const override { return (operation); }                                           \
                                                                                                \
    static auto create(std::shared_ptr<MathOp<T>> x)                                            \
    {                                                                                           \
        return std::shared_ptr<op_name<T>>(new op_name<T>(x));                                  \
    }                                                                                           \
                                                                                                \
protected:                                                                                      \
    VisitorResult<T> accept(Visitor<T>& visitor) override                                       \
    {                                                                                           \
        return visitor.visit(std::static_pointer_cast<op_name<T>>(this->shared_from_this()));   \
    }                                                                                           \
                                                                                                \
private:                                                                                        \
    op_name(std::shared_ptr<MathOp<T>> x)                                                       \
        : MathUnaryOp<T>(x)                                                                     \
    { }                                                                                         \
}

DEFINE_UNARY_OP(Negate,     -this->x->result());
DEFINE_UNARY_OP(Sqrt,  sqrt (this->x->result()));
DEFINE_UNARY_OP(Log,   log  (this->x->result()));
DEFINE_UNARY_OP(Log10, log10(this->x->result()));
DEFINE_UNARY_OP(Sin,   sin  (this->x->result()));
DEFINE_UNARY_OP(Cos,   cos  (this->x->result()));
DEFINE_UNARY_OP(Tan,   tan  (this->x->result()));
DEFINE_UNARY_OP(ASin,  asin (this->x->result()));
DEFINE_UNARY_OP(ACos,  acos (this->x->result()));
DEFINE_UNARY_OP(ATan,  atan (this->x->result()));
DEFINE_UNARY_OP(Sinh,  sinh (this->x->result()));
DEFINE_UNARY_OP(Cosh,  cosh (this->x->result()));
DEFINE_UNARY_OP(Tanh,  tanh (this->x->result()));
DEFINE_UNARY_OP(ASinh, asinh(this->x->result()));
DEFINE_UNARY_OP(ACosh, acosh(this->x->result()));
DEFINE_UNARY_OP(ATanh, atanh(this->x->result()));

#undef DEFINE_UNARY_OP

template<typename T> std::shared_ptr<MathOp<T>> sqrt(std::shared_ptr<MathOp<T>> x)   { return Sqrt<T>::create(x); }
template<typename T> std::shared_ptr<MathOp<T>> log(std::shared_ptr<MathOp<T>> x)    { return Log<T>::create(x); }
template<typename T> std::shared_ptr<MathOp<T>> log10(std::shared_ptr<MathOp<T>> x)  { return Log10<T>::create(x); }
template<typename T> std::shared_ptr<MathOp<T>> sin(std::shared_ptr<MathOp<T>> x)    { return Sin<T>::create(x); }
template<typename T> std::shared_ptr<MathOp<T>> asin(std::shared_ptr<MathOp<T>> x)   { return ASin<T>::create(x); }
template<typename T> std::shared_ptr<MathOp<T>> cos(std::shared_ptr<MathOp<T>> x)    { return Cos<T>::create(x); }
template<typename T> std::shared_ptr<MathOp<T>> acos(std::shared_ptr<MathOp<T>> x)   { return ACos<T>::create(x); }
template<typename T> std::shared_ptr<MathOp<T>> tan(std::shared_ptr<MathOp<T>> x)    { return Tan<T>::create(x); }
template<typename T> std::shared_ptr<MathOp<T>> atan(std::shared_ptr<MathOp<T>> x)   { return ATan<T>::create(x); }
template<typename T> std::shared_ptr<MathOp<T>> sinh(std::shared_ptr<MathOp<T>> x)   { return Sinh<T>::create(x); }
template<typename T> std::shared_ptr<MathOp<T>> asinh(std::shared_ptr<MathOp<T>> x)  { return ASinh<T>::create(x); }
template<typename T> std::shared_ptr<MathOp<T>> cosh(std::shared_ptr<MathOp<T>> x)   { return Cosh<T>::create(x); }
template<typename T> std::shared_ptr<MathOp<T>> acosh(std::shared_ptr<MathOp<T>> x)  { return ACosh<T>::create(x); }
template<typename T> std::shared_ptr<MathOp<T>> tanh(std::shared_ptr<MathOp<T>> x)   { return Tanh<T>::create(x); }
template<typename T> std::shared_ptr<MathOp<T>> atanh(std::shared_ptr<MathOp<T>> x)  { return ATanh<T>::create(x); }

/* Binary math operations */
#define DEFINE_BINARY_OP(op_name, operation, commutative, right_assoc, bodmas)                  \
template<typename T>                                                                            \
struct op_name : public MathBinaryOp<T>                                                         \
{                                                                                               \
    T result() const override { return (operation); }                                           \
                                                                                                \
    static auto create(std::shared_ptr<MathOp<T>>lhs, std::shared_ptr<MathOp<T>>rhs)            \
    {                                                                                           \
        return std::shared_ptr<op_name<T>>(new op_name<T>(lhs, rhs));                           \
    }                                                                                           \
                                                                                                \
    bool is_commutative() const override { return commutative; }                                \
    bool right_associative() const override { return right_assoc; }                             \
                                                                                                \
protected:                                                                                      \
    VisitorResult<T> accept(Visitor<T>& visitor) override                                       \
    {                                                                                           \
        return visitor.visit(std::static_pointer_cast<op_name<T>>(this->shared_from_this()));   \
    }                                                                                           \
                                                                                                \
private:                                                                                        \
    op_name(std::shared_ptr<MathOp<T>>lhs, std::shared_ptr<MathOp<T>>rhs)                       \
        : MathBinaryOp<T>(lhs, rhs, bodmas)                                                     \
    { }                                                                                         \
};

DEFINE_BINARY_OP(Pow, pow(this->lhs->result(),  this->rhs->result()), false,  true, Bodmas::Exponents);
DEFINE_BINARY_OP(Mul,    (this->lhs->result() * this->rhs->result()), true,  false, Bodmas::MultiplicationDivision);
DEFINE_BINARY_OP(Div,    (this->lhs->result() / this->rhs->result()), false, false, Bodmas::MultiplicationDivision);
DEFINE_BINARY_OP(Add,    (this->lhs->result() + this->rhs->result()), true,  false, Bodmas::AdditionSubtraction);
DEFINE_BINARY_OP(Sub,    (this->lhs->result() - this->rhs->result()), false, false, Bodmas::AdditionSubtraction);

#undef DEFINE_BINARY_OP

} /* namespace MathOps */

#endif /*ALGEBLAH_H */
