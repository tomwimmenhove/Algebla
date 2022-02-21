#ifndef DEFAULTFORMATTER_H
#define DEFAULTFORMATTER_H

#include "algeblah.h"

#include <sstream>
#include <iomanip>

namespace MathOps
{

template<typename T>
struct DefaultFormatter : Visitor<T>
{
    DefaultFormatter(int precision)
     : DefaultFormatter(precision, false)
    { }

    VisitorResult<T> visit(std::shared_ptr<ConstantSymbol<T>> op) override { return op->get_name(); }
    VisitorResult<T> visit(std::shared_ptr<Variable<T>> op) override { return op->get_name(); }
    VisitorResult<T> visit(std::shared_ptr<ValueVariable<T>> op) override { return value_to_string(op->result()); }
    VisitorResult<T> visit(std::shared_ptr<NamedConstant<T>> op) override { return op->get_name(); }
    VisitorResult<T> visit(std::shared_ptr<MutableValue<T>> op) override { return value_to_string(op->result()); }
    VisitorResult<T> visit(std::shared_ptr<ConstantValue<T>> op) override { return value_to_string(op->result()); }

    VisitorResult<T> visit(std::shared_ptr<Container<T>> op) override { return op->get_name(); }

    VisitorResult<T> visit(std::shared_ptr<Negate<T>> op) override { return str_unary_sign(op, op->get_x(), "-"); }
    VisitorResult<T> visit(std::shared_ptr<Sqrt<T>> op) override { return str_unary(op->get_x(), "sqrt"); }
    VisitorResult<T> visit(std::shared_ptr<Log<T>> op) override { return str_unary(op->get_x(), "log"); }
    VisitorResult<T> visit(std::shared_ptr<Log10<T>> op) override { return str_unary(op->get_x(), "log10"); }
    VisitorResult<T> visit(std::shared_ptr<Sin<T>> op) override { return str_unary(op->get_x(), "sin"); }
    VisitorResult<T> visit(std::shared_ptr<ASin<T>> op) override { return str_unary(op->get_x(), "asin"); }
    VisitorResult<T> visit(std::shared_ptr<Cos<T>> op) override { return str_unary(op->get_x(), "cos"); }
    VisitorResult<T> visit(std::shared_ptr<ACos<T>> op) override { return str_unary(op->get_x(), "acos"); }
    VisitorResult<T> visit(std::shared_ptr<Tan<T>> op) override { return str_unary(op->get_x(), "tan"); }
    VisitorResult<T> visit(std::shared_ptr<ATan<T>> op) override { return str_unary(op->get_x(), "atan"); }
    VisitorResult<T> visit(std::shared_ptr<Sinh<T>> op) override { return str_unary(op->get_x(), "sinh"); }
    VisitorResult<T> visit(std::shared_ptr<ASinh<T>> op) override { return str_unary(op->get_x(), "asinh"); }
    VisitorResult<T> visit(std::shared_ptr<Cosh<T>> op) override { return str_unary(op->get_x(), "cosh"); }
    VisitorResult<T> visit(std::shared_ptr<ACosh<T>> op) override { return str_unary(op->get_x(), "acosh"); }
    VisitorResult<T> visit(std::shared_ptr<Tanh<T>> op) override { return str_unary(op->get_x(), "tanh"); }
    VisitorResult<T> visit(std::shared_ptr<ATanh<T>> op) override { return str_unary(op->get_x(), "atanh"); }

    VisitorResult<T> visit(std::shared_ptr<Pow<T>> op) override { return str_binary(op, op->get_lhs(), op->get_rhs(), " ^ "); }
    VisitorResult<T> visit(std::shared_ptr<Mul<T>> op) override { return str_binary(op, op->get_lhs(), op->get_rhs(), " * "); }
    VisitorResult<T> visit(std::shared_ptr<Div<T>> op) override { return str_binary(op, op->get_lhs(), op->get_rhs(), " / "); }
    VisitorResult<T> visit(std::shared_ptr<Add<T>> op) override { return str_binary(op, op->get_lhs(), op->get_rhs(), " + "); }
    VisitorResult<T> visit(std::shared_ptr<Sub<T>> op) override { return str_binary(op, op->get_lhs(), op->get_rhs(), " - "); }

private:
    DefaultFormatter(int precision, bool parenthesize)
     : precision(precision), parenthesize(parenthesize)
    { }

    const int precision;
    const bool parenthesize;

    std::string value_to_string(T x) const
    {
        std::stringstream ss;
        ss << std::setprecision(precision) << x;
        return ss.str();        
    }

    std::string str_binary(std::shared_ptr<MathOp<T>> op,
        std::shared_ptr<MathOp<T>> lhs, std::shared_ptr<MathOp<T>> rhs, std::string symbol)
    {
        bool right_associative = op->right_associative();
        std::stringstream ss;

        if (parenthesize)
        {
            ss << '(';
        }

        side_to_stream(ss, op, lhs, right_associative);
        ss << symbol;
        side_to_stream(ss, op, rhs, !right_associative);

        if (parenthesize)
        {
            ss << ')';
        }

        return ss.str();
    }

    void side_to_stream(std::stringstream& ss, std::shared_ptr<MathOp<T>> op, std::shared_ptr<MathOp<T>> side,
        bool use_commutation)
    {
        Bodmas parent_precedence = op->precedence();
        bool use_parens = !use_commutation || op->is_commutative()
            ? parent_precedence < side->precedence()
            : parent_precedence <= side->precedence();

        ss << side->format(DefaultFormatter(precision, use_parens));
    }

    std::string str_unary_sign(std::shared_ptr<MathOp<T>> op,
        std::shared_ptr<MathOp<T>> x, std::string symbol)
    {
        std::stringstream ss;

        if (parenthesize)
        {
            ss << '(';
        }

        ss << symbol;

        side_to_stream(ss, op, x, true);

        if (parenthesize)
        {
            ss << ')';
        }

        return ss.str();
    }

    std::string str_unary(std::shared_ptr<MathOp<T>> x, std::string symbol)
    {
        return symbol + "(" + x->format(*this) + ")";            
    }
};

} /* namespace MathOps */

#endif /* DEFAULTFORMATTER_H */