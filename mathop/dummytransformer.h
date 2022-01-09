#ifndef DUMMYTRANSFORMER_H
#define DUMMYTRANSFORMER_H

#include "algeblah.h"

namespace MathOps
{

template <typename T>
struct MathOpDummyTransformer : public MathOpTransformer<T>
{
    virtual std::shared_ptr<MathOp<T>> visit(std::shared_ptr<MathOpMutableSymbol<T>> op) override { return op; }
    virtual std::shared_ptr<MathOp<T>> visit(std::shared_ptr<MathOpConstantSymbol<T>> op) override { return op; }
    virtual std::shared_ptr<MathOp<T>> visit(std::shared_ptr<MathOpVariable<T>> op) override { return op; }
    virtual std::shared_ptr<MathOp<T>> visit(std::shared_ptr<MathOpValueVariable<T>> op) override { return op; }
    virtual std::shared_ptr<MathOp<T>> visit(std::shared_ptr<MathOpNamedConstant<T>> op) override { return op; }
    virtual std::shared_ptr<MathOp<T>> visit(std::shared_ptr<MathOpMutableValue<T>> op) override { return op; }
    virtual std::shared_ptr<MathOp<T>> visit(std::shared_ptr<MathOpConstantValue<T>> op) override { return op; }

    std::shared_ptr<MathOp<T>> visit(std::shared_ptr<MathOpNegate<T>> op) override
    {
        return -(op->get_x()->transform(*this));
    }

    virtual std::shared_ptr<MathOp<T>> visit(std::shared_ptr<MathOpSqrt<T>> op) override
    {
        return sqrt(op->get_x()->transform(*this));
    }

    virtual std::shared_ptr<MathOp<T>> visit(std::shared_ptr<MathOpSquare<T>> op) override
    {
        return square(op->get_x()->transform(*this));
    }

    virtual std::shared_ptr<MathOp<T>> visit(std::shared_ptr<MathOpLog<T>> op) override
    {
        return log(op->get_x()->transform(*this));
    }

    virtual std::shared_ptr<MathOp<T>> visit(std::shared_ptr<MathOpSin<T>> op) override
    {
        return sin(op->get_x()->transform(*this));
    }

    virtual std::shared_ptr<MathOp<T>> visit(std::shared_ptr<MathOpASin<T>> op) override
    {
        return asin(op->get_x()->transform(*this));
    }

    virtual std::shared_ptr<MathOp<T>> visit(std::shared_ptr<MathOpCos<T>> op) override
    {
        return cos(op->get_x()->transform(*this));
    }

    virtual std::shared_ptr<MathOp<T>> visit(std::shared_ptr<MathOpACos<T>> op) override
    {
        return acos(op->get_x()->transform(*this));
    }

    virtual std::shared_ptr<MathOp<T>> visit(std::shared_ptr<MathOpTan<T>> op) override
    {
        return tan(op->get_x()->transform(*this));
    }

    virtual std::shared_ptr<MathOp<T>> visit(std::shared_ptr<MathOpATan<T>> op) override
    {
        return atan(op->get_x()->transform(*this));
    }

    virtual std::shared_ptr<MathOp<T>> visit(std::shared_ptr<MathOpPow<T>> op) override
    {
        return op->get_lhs()->transform(*this) ^ op->get_rhs()->transform(*this);
    }

    virtual std::shared_ptr<MathOp<T>> visit(std::shared_ptr<MathOpMul<T>> op) override
    {
        return op->get_lhs()->transform(*this) * op->get_rhs()->transform(*this);
    }

    virtual std::shared_ptr<MathOp<T>> visit(std::shared_ptr<MathOpDiv<T>> op) override
    {
        return op->get_lhs()->transform(*this) / op->get_rhs()->transform(*this);
    }

    virtual std::shared_ptr<MathOp<T>> visit(std::shared_ptr<MathOpAdd<T>> op) override
    {
        return op->get_lhs()->transform(*this) + op->get_rhs()->transform(*this);
    }

    virtual std::shared_ptr<MathOp<T>> visit(std::shared_ptr<MathOpSub<T>> op) override
    {
        return op->get_lhs()->transform(*this) - op->get_rhs()->transform(*this);
    }
};

} /* namespace MathOps */

#endif /* DUMMYTRANSFORMER_H */