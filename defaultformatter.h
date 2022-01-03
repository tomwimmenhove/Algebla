#ifndef DEFAULTFORMATTER_H
#define DEFAULTFORMATTER_H

#include "algeblah.h"

#include <sstream>

template<typename T>
struct MathOpDefaultFormatter : MathOpFormatter<T>
{
    virtual std::string visit(std::shared_ptr<MathOpMutableSymbol<T>> op) override { return op->str(); }
    virtual std::string visit(std::shared_ptr<MathOpConstantSymbol<T>> op) override { return op->str(); }
    virtual std::string visit(std::shared_ptr<MathOpVariable<T>> op) override { return op->str(); }
    virtual std::string visit(std::shared_ptr<MathOpValueVariable<T>> op) override { return op->str(); }
    virtual std::string visit(std::shared_ptr<MathOpNamedConstant<T>> op) override { return op->str(); }
    virtual std::string visit(std::shared_ptr<MathOpMutableValue<T>> op) override { return op->str(); }
    virtual std::string visit(std::shared_ptr<MathOpConstantValue<T>> op) override { return op->str(); }

    virtual std::string visit(std::shared_ptr<MathOpSqrt<T>> op) override { return str_unary(op->get_x(), "sqrt"); }
    virtual std::string visit(std::shared_ptr<MathOpSquare<T>> op) override { return str_unary_postfix(op->get_x(), "^2"); }
    virtual std::string visit(std::shared_ptr<MathOpLog<T>> op) override { return str_unary(op->get_x(), "log"); }
    virtual std::string visit(std::shared_ptr<MathOpSin<T>> op) override { return str_unary(op->get_x(), "sin"); }
    virtual std::string visit(std::shared_ptr<MathOpASin<T>> op) override { return str_unary(op->get_x(), "asin"); }
    virtual std::string visit(std::shared_ptr<MathOpCos<T>> op) override { return str_unary(op->get_x(), "cos"); }
    virtual std::string visit(std::shared_ptr<MathOpACos<T>> op) override { return str_unary(op->get_x(), "acos"); }
    virtual std::string visit(std::shared_ptr<MathOpTan<T>> op) override { return str_unary(op->get_x(), "tan"); }
    virtual std::string visit(std::shared_ptr<MathOpATan<T>> op) override { return str_unary(op->get_x(), "atan"); }

    virtual std::string visit(std::shared_ptr<MathOpPow<T>> op) override { return str_binary(op, op->get_lhs(), op->get_rhs(), " ^ "); }
    virtual std::string visit(std::shared_ptr<MathOpMul<T>> op) override { return str_binary(op, op->get_lhs(), op->get_rhs(), " * "); }
    virtual std::string visit(std::shared_ptr<MathOpDiv<T>> op) override { return str_binary(op, op->get_lhs(), op->get_rhs(), " / "); }
    virtual std::string visit(std::shared_ptr<MathOpAdd<T>> op) override { return str_binary(op, op->get_lhs(), op->get_rhs(), " + "); }
    virtual std::string visit(std::shared_ptr<MathOpSub<T>> op) override { return str_binary(op, op->get_lhs(), op->get_rhs(), " - "); }

private:
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
        int parent_precedence = op->precedence();
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

#endif /* DEFAULTFORMATTER_H */