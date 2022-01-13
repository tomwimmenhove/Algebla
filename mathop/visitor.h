#ifndef VISITOR_H
#define VISITOR_H

#include <string>
#include <memory>
#include <variant>

namespace MathOps
{

/* Forward declarations */
template<typename T> struct MathOp;
template<typename T> struct ConstantSymbol;
template<typename T> struct Variable;
template<typename T> struct ValueVariable;
template<typename T> struct NamedConstant;
template<typename T> struct MutableValue;
template<typename T> struct ConstantValue;
template<typename T> struct Negate;
template<typename T> struct Sqrt;
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

template <typename T>
using VisitorResult = std::variant<int, std::string, std::shared_ptr<MathOp<T>>>;

template<typename T>
struct Visitor
{
    virtual VisitorResult<T> visit(std::shared_ptr<ConstantSymbol<T>> op) = 0;
    virtual VisitorResult<T> visit(std::shared_ptr<Variable<T>> op) = 0;
    virtual VisitorResult<T> visit(std::shared_ptr<ValueVariable<T>> op) = 0;
    virtual VisitorResult<T> visit(std::shared_ptr<NamedConstant<T>> op) = 0;
    virtual VisitorResult<T> visit(std::shared_ptr<MutableValue<T>> op) = 0;
    virtual VisitorResult<T> visit(std::shared_ptr<ConstantValue<T>> op) = 0;
    virtual VisitorResult<T> visit(std::shared_ptr<Negate<T>> op) = 0;
    virtual VisitorResult<T> visit(std::shared_ptr<Sqrt<T>> op) = 0;
    virtual VisitorResult<T> visit(std::shared_ptr<Log<T>> op) = 0;
    virtual VisitorResult<T> visit(std::shared_ptr<Sin<T>> op) = 0;
    virtual VisitorResult<T> visit(std::shared_ptr<ASin<T>> op) = 0;
    virtual VisitorResult<T> visit(std::shared_ptr<Cos<T>> op) = 0;
    virtual VisitorResult<T> visit(std::shared_ptr<ACos<T>> op) = 0;
    virtual VisitorResult<T> visit(std::shared_ptr<Tan<T>> op) = 0;
    virtual VisitorResult<T> visit(std::shared_ptr<ATan<T>> op) = 0;
    virtual VisitorResult<T> visit(std::shared_ptr<Pow<T>> op) = 0;
    virtual VisitorResult<T> visit(std::shared_ptr<Mul<T>> op) = 0;
    virtual VisitorResult<T> visit(std::shared_ptr<Div<T>> op) = 0;
    virtual VisitorResult<T> visit(std::shared_ptr<Add<T>> op) = 0;
    virtual VisitorResult<T> visit(std::shared_ptr<Sub<T>> op) = 0;
    virtual ~Visitor() {}
};

} /* MathOps */

#endif /* VISITOR_H */