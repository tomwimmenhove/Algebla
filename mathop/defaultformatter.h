#ifndef DEFAULTFORMATTER_H
#define DEFAULTFORMATTER_H

#include "algeblah.h"

#include <sstream>
#include <iomanip>

namespace MathOps
{

template<typename T>
struct DefaultFormatter : Formatter<T>
{
    DefaultFormatter(int precision = 5)
     : precision(precision)
    { }    

    std::string visit(std::shared_ptr<MutableSymbol<T>> op) override { return op->get_symbol(); }
    std::string visit(std::shared_ptr<ConstantSymbol<T>> op) override { return op->get_symbol(); }
    std::string visit(std::shared_ptr<OpVariable<T>> op) override { return op->get_symbol(); }
    std::string visit(std::shared_ptr<ValueVariable<T>> op) override { return value_to_string(op->result()); }
    std::string visit(std::shared_ptr<NamedConstant<T>> op) override { return op->get_symbol(); }
    std::string visit(std::shared_ptr<MutableValue<T>> op) override { return value_to_string(op->result()); }
    std::string visit(std::shared_ptr<ConstantValue<T>> op) override { return value_to_string(op->result()); }

    std::string visit(std::shared_ptr<Negate<T>> op) override { return str_unary(op->get_x(), "-"); }
    std::string visit(std::shared_ptr<Sqrt<T>> op) override { return str_unary(op->get_x(), "sqrt"); }
    std::string visit(std::shared_ptr<Square<T>> op) override { return str_unary_postfix(op->get_x(), "^2"); }
    std::string visit(std::shared_ptr<Log<T>> op) override { return str_unary(op->get_x(), "log"); }
    std::string visit(std::shared_ptr<Sin<T>> op) override { return str_unary(op->get_x(), "sin"); }
    std::string visit(std::shared_ptr<ASin<T>> op) override { return str_unary(op->get_x(), "asin"); }
    std::string visit(std::shared_ptr<Cos<T>> op) override { return str_unary(op->get_x(), "cos"); }
    std::string visit(std::shared_ptr<ACos<T>> op) override { return str_unary(op->get_x(), "acos"); }
    std::string visit(std::shared_ptr<Tan<T>> op) override { return str_unary(op->get_x(), "tan"); }
    std::string visit(std::shared_ptr<ATan<T>> op) override { return str_unary(op->get_x(), "atan"); }

    std::string visit(std::shared_ptr<Pow<T>> op) override { return str_binary(op, op->get_lhs(), op->get_rhs(), " ^ "); }
    std::string visit(std::shared_ptr<Mul<T>> op) override { return str_binary(op, op->get_lhs(), op->get_rhs(), " * "); }
    std::string visit(std::shared_ptr<Div<T>> op) override { return str_binary(op, op->get_lhs(), op->get_rhs(), " / "); }
    std::string visit(std::shared_ptr<Add<T>> op) override { return str_binary(op, op->get_lhs(), op->get_rhs(), " + "); }
    std::string visit(std::shared_ptr<Sub<T>> op) override { return str_binary(op, op->get_lhs(), op->get_rhs(), " - "); }

private:
    int precision;

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

        side_to_stream(ss, op, lhs, right_associative);
        ss << symbol;
        side_to_stream(ss, op, rhs, !right_associative);
        
        return ss.str();
    }

    void side_to_stream(std::stringstream& ss, std::shared_ptr<MathOp<T>> op, std::shared_ptr<MathOp<T>> side,
        bool use_commutation)
    {
        Bodmas parent_precedence = op->precedence();
        bool use_parens = !use_commutation || op->is_commutative()
            ? parent_precedence < side->precedence()
            : parent_precedence <= side->precedence();

        if (use_parens)
        {
            ss << '(';
        }

        ss << side->format(*this);

        if (use_parens)
        {
            ss << ')';
        }
    }

    std::string str_unary(std::shared_ptr<MathOp<T>> x, std::string symbol)
    {
        std::stringstream ss;

        ss << symbol << '(' << x->format(*this) << ')';
        
        return ss.str();
    }

    std::string str_unary_postfix(std::shared_ptr<MathOp<T>> x, std::string postfix)
    {
        std::stringstream ss;

        ss << '(' << x->format(*this) << ')' << postfix;
        
        return ss.str();
    }
};

} /* namespace MathOps */

#endif /* DEFAULTFORMATTER_H */