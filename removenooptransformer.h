#ifndef REMOVENOOPVISITOR_H
#define REMOVENOOPVISITOR_H

#include "algeblah.h"

template <typename T>
struct MathOpRemoveNoOpTransformer : public MathOpTransformer<T>
{
    std::shared_ptr<MathOp<T>> visit(std::shared_ptr<MathOpMutableSymbol<T>> op) override { return op; }
    std::shared_ptr<MathOp<T>> visit(std::shared_ptr<MathOpConstantSymbol<T>> op) override { return op; }
    std::shared_ptr<MathOp<T>> visit(std::shared_ptr<MathOpVariable<T>> op) override { return op; }
    std::shared_ptr<MathOp<T>> visit(std::shared_ptr<MathOpValueVariable<T>> op) override { return op; }
    std::shared_ptr<MathOp<T>> visit(std::shared_ptr<MathOpNamedConstant<T>> op) override { return op; }
    std::shared_ptr<MathOp<T>> visit(std::shared_ptr<MathOpMutableValue<T>> op) override { return op; }
    std::shared_ptr<MathOp<T>> visit(std::shared_ptr<MathOpConstantValue<T>> op) override { return op; }

    std::shared_ptr<MathOp<T>> visit(std::shared_ptr<MathOpSqrt<T>> op) override
    {
        return sqrt(op->get_x()->transform(*this));
    }

    std::shared_ptr<MathOp<T>> visit(std::shared_ptr<MathOpSquare<T>> op) override
    {
        return square(op->get_x()->transform(*this));
    }

    std::shared_ptr<MathOp<T>> visit(std::shared_ptr<MathOpLog<T>> op) override
    {
        return log(op->get_x()->transform(*this));
    }

    std::shared_ptr<MathOp<T>> visit(std::shared_ptr<MathOpSin<T>> op) override
    {
        return sin(op->get_x()->transform(*this));
    }

    std::shared_ptr<MathOp<T>> visit(std::shared_ptr<MathOpASin<T>> op) override
    {
        return asin(op->get_x()->transform(*this));
    }

    std::shared_ptr<MathOp<T>> visit(std::shared_ptr<MathOpCos<T>> op) override
    {
        return cos(op->get_x()->transform(*this));
    }

    std::shared_ptr<MathOp<T>> visit(std::shared_ptr<MathOpACos<T>> op) override
    {
        return acos(op->get_x()->transform(*this));
    }

    std::shared_ptr<MathOp<T>> visit(std::shared_ptr<MathOpTan<T>> op) override
    {
        return tan(op->get_x()->transform(*this));
    }

    std::shared_ptr<MathOp<T>> visit(std::shared_ptr<MathOpATan<T>> op) override
    {
        return atan(op->get_x()->transform(*this));
    }

    std::shared_ptr<MathOp<T>> visit(std::shared_ptr<MathOpPow<T>> op) override
    {
        auto lhs = op->get_lhs()->transform(*this);
        auto rhs = op->get_rhs()->transform(*this);

        if (lhs->is_constant() && lhs->result() == 0)
        {
            return MathFactory::ConstantValue<T>(0.0);
        }

        if (rhs->is_constant() && rhs->result() == 0)
        {
            return MathFactory::ConstantValue<T>(1.0);
        }

        if (rhs->is_constant() && rhs->result() == 1)
        {
            return lhs;
        }

        return lhs ^ rhs;
    }

    std::shared_ptr<MathOp<T>> visit(std::shared_ptr<MathOpMul<T>> op) override
    {
        auto lhs = op->get_lhs()->transform(*this);
        auto rhs = op->get_rhs()->transform(*this);

        if (lhs->is_constant() && lhs->result() == 1)
        {
            return rhs;
        }

        if (rhs->is_constant() && rhs->result() == 1)
        {
            return lhs;
        }

        if ((lhs->is_constant() && lhs->result() == 0) ||
            (rhs->is_constant() && rhs->result() == 0))
        {
            return MathFactory::ConstantValue<T>(0.0);
        }

        return lhs  * rhs;
    }

    std::shared_ptr<MathOp<T>> visit(std::shared_ptr<MathOpDiv<T>> op) override
    {
        auto lhs = op->get_lhs()->transform(*this);
        auto rhs = op->get_rhs()->transform(*this);

        if (lhs->is_constant() && rhs->is_constant() && lhs->result() == rhs->result())
        {
            return MathFactory::ConstantValue<T>(1.0);
        }

        if (rhs->is_constant() && rhs->result() == 1)
        {
            return lhs;
        }

        if (lhs->is_constant() && lhs->result() == 0)
        {
            return MathFactory::ConstantValue<T>(0.0);
        }

        return lhs / rhs;
    }

    std::shared_ptr<MathOp<T>> visit(std::shared_ptr<MathOpAdd<T>> op) override
    {
        auto lhs = op->get_lhs()->transform(*this);
        auto rhs = op->get_rhs()->transform(*this);

        if (lhs->is_constant() && lhs->result() == 0)
        {
            return rhs;
        }

        if (rhs->is_constant() && rhs->result() == 0)
        {
            return lhs;
        }

        return lhs + rhs;
    }

    std::shared_ptr<MathOp<T>> visit(std::shared_ptr<MathOpSub<T>> op) override
    {
        auto lhs = op->get_lhs()->transform(*this);
        auto rhs = op->get_rhs()->transform(*this);

        if (rhs->is_constant() && rhs->result() == 0)
        {
            return lhs;
        }

        return lhs - rhs;
    }
};
#endif /* REMOVENOOPVISITOR_H */