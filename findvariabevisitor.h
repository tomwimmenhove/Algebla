#ifndef FINDVARIABLEVISITOR_H
#define FINDVARIABLEVISITOR_H

#include "algeblah.h"

template <typename T>
struct MathOpFindVariableVisitor : public MathOpVisitor<T>
{
    MathOpFindVariableVisitor(std::string symbol)
        : symbol(symbol)
    { }

    virtual std::shared_ptr<MathOp<T>> visit(std::shared_ptr<MathOpMutableSymbol<T>> op) override { return nullptr; }
    virtual std::shared_ptr<MathOp<T>> visit(std::shared_ptr<MathOpConstantSymbol<T>> op) override { return nullptr; }
    virtual std::shared_ptr<MathOp<T>> visit(std::shared_ptr<MathOpMutableValue<T>> op) override { return nullptr; }
    virtual std::shared_ptr<MathOp<T>> visit(std::shared_ptr<MathOpConstantValue<T>> op) override { return nullptr; }

    virtual std::shared_ptr<MathOp<T>> visit(std::shared_ptr<MathOpVariable<T>> op) override
    {
        return op->get_symbol() == symbol ? op : nullptr;
    }

    virtual std::shared_ptr<MathOp<T>> visit(std::shared_ptr<MathOpSqrt<T>> op) override
    {
        return op->get_x()->accept(this);
    }

    virtual std::shared_ptr<MathOp<T>> visit(std::shared_ptr<MathOpSquare<T>> op) override
    {
        return op->get_x()->accept(this);
    }

    virtual std::shared_ptr<MathOp<T>> visit(std::shared_ptr<MathOpLog<T>> op) override
    {
        return op->get_x()->accept(this);
    }

    virtual std::shared_ptr<MathOp<T>> visit(std::shared_ptr<MathOpSin<T>> op) override
    {
        return op->get_x()->accept(this);
    }

    virtual std::shared_ptr<MathOp<T>> visit(std::shared_ptr<MathOpASin<T>> op) override
    {
        return op->get_x()->accept(this);
    }

    virtual std::shared_ptr<MathOp<T>> visit(std::shared_ptr<MathOpCos<T>> op) override
    {
        return op->get_x()->accept(this);
    }

    virtual std::shared_ptr<MathOp<T>> visit(std::shared_ptr<MathOpACos<T>> op) override
    {
        return op->get_x()->accept(this);
    }

    virtual std::shared_ptr<MathOp<T>> visit(std::shared_ptr<MathOpTan<T>> op) override
    {
        return op->get_x()->accept(this);
    }

    virtual std::shared_ptr<MathOp<T>> visit(std::shared_ptr<MathOpATan<T>> op) override
    {
        return op->get_x()->accept(this);
    }

    virtual std::shared_ptr<MathOp<T>> visit(std::shared_ptr<MathOpPow<T>> op) override
    {
        auto lhs = op->get_lhs()->accept(this);
        return lhs ? lhs : op->get_lhs()->accept(this);
    }

    virtual std::shared_ptr<MathOp<T>> visit(std::shared_ptr<MathOpMul<T>> op) override
    {
        auto lhs = op->get_lhs()->accept(this);
        return lhs ? lhs : op->get_lhs()->accept(this);
    }

    virtual std::shared_ptr<MathOp<T>> visit(std::shared_ptr<MathOpDiv<T>> op) override
    {
        auto lhs = op->get_lhs()->accept(this);
        return lhs ? lhs : op->get_lhs()->accept(this);
    }

    virtual std::shared_ptr<MathOp<T>> visit(std::shared_ptr<MathOpAdd<T>> op) override
    {
        auto lhs = op->get_lhs()->accept(this);
        return lhs ? lhs : op->get_lhs()->accept(this);
    }

    virtual std::shared_ptr<MathOp<T>> visit(std::shared_ptr<MathOpSub<T>> op) override
    {
        auto lhs = op->get_lhs()->accept(this);
        return lhs ? lhs : op->get_lhs()->accept(this);
    }

private:
    std::string symbol;
};

#endif /* FINDVARIABLEVISITOR_H */