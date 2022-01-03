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
        int precedence = op->precedence();
        int lhs_precedence = lhs->precedence();
        int rhs_precedence = rhs->precedence();
        int is_commutative = op->is_commutative();
        bool right_associative = op->right_associative();

        bool left_paren = !right_associative || is_commutative ? precedence < lhs->precedence() : precedence <= lhs->precedence();
        bool right_paren = right_associative || is_commutative ? precedence < rhs->precedence() : precedence <= rhs->precedence();

        std::stringstream ss;

        if (left_paren) ss << '(';
        ss << lhs->format(*this);
        if (left_paren) ss << ')';

        ss << symbol;

        if (right_paren) ss << '(';
        ss << rhs->format(*this);
        if (right_paren) ss << ')';
        
        return ss.str();
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