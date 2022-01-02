#ifndef FINDVARIABLETRANSFORMER_H
#define FINDVARIABLETRANSFORMER_H

#include "algeblah.h"

template <typename T>
struct MathOpFindVariableTransformer : public MathOpTransformer<T>
{
    MathOpFindVariableTransformer(std::string symbol)
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
        return op->get_x()->transform(*this);
    }

    virtual std::shared_ptr<MathOp<T>> visit(std::shared_ptr<MathOpSquare<T>> op) override
    {
        return op->get_x()->transform(*this);
    }

    virtual std::shared_ptr<MathOp<T>> visit(std::shared_ptr<MathOpLog<T>> op) override
    {
        return op->get_x()->transform(*this);
    }

    virtual std::shared_ptr<MathOp<T>> visit(std::shared_ptr<MathOpSin<T>> op) override
    {
        return op->get_x()->transform(*this);
    }

    virtual std::shared_ptr<MathOp<T>> visit(std::shared_ptr<MathOpASin<T>> op) override
    {
        return op->get_x()->transform(*this);
    }

    virtual std::shared_ptr<MathOp<T>> visit(std::shared_ptr<MathOpCos<T>> op) override
    {
        return op->get_x()->transform(*this);
    }

    virtual std::shared_ptr<MathOp<T>> visit(std::shared_ptr<MathOpACos<T>> op) override
    {
        return op->get_x()->transform(*this);
    }

    virtual std::shared_ptr<MathOp<T>> visit(std::shared_ptr<MathOpTan<T>> op) override
    {
        return op->get_x()->transform(*this);
    }

    virtual std::shared_ptr<MathOp<T>> visit(std::shared_ptr<MathOpATan<T>> op) override
    {
        return op->get_x()->transform(*this);
    }

    virtual std::shared_ptr<MathOp<T>> visit(std::shared_ptr<MathOpPow<T>> op) override
    {
        auto lhs = op->get_lhs()->transform(*this);
        return lhs ? lhs : op->get_rhs()->transform(*this);
    }

    virtual std::shared_ptr<MathOp<T>> visit(std::shared_ptr<MathOpMul<T>> op) override
    {
        auto lhs = op->get_lhs()->transform(*this);
        return lhs ? lhs : op->get_rhs()->transform(*this);
    }

    virtual std::shared_ptr<MathOp<T>> visit(std::shared_ptr<MathOpDiv<T>> op) override
    {
        auto lhs = op->get_lhs()->transform(*this);
        return lhs ? lhs : op->get_rhs()->transform(*this);
    }

    virtual std::shared_ptr<MathOp<T>> visit(std::shared_ptr<MathOpAdd<T>> op) override
    {
        auto lhs = op->get_lhs()->transform(*this);
        return lhs ? lhs : op->get_rhs()->transform(*this);
    }

    virtual std::shared_ptr<MathOp<T>> visit(std::shared_ptr<MathOpSub<T>> op) override
    {
        auto lhs = op->get_lhs()->transform(*this);
        return lhs ? lhs : op->get_rhs()->transform(*this);
    }

private:
    std::string symbol;
};

#endif /* FINDVARIABLETRANSFORMER_H */