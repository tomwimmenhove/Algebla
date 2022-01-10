#ifndef REMOVENOOPVISITOR_H
#define REMOVENOOPVISITOR_H

#include "algeblah.h"

namespace MathOps
{

template <typename T>
struct MathOpRemoveNoOpTransformer : public Transformer<T>
{
    std::shared_ptr<MathOp<T>> visit(std::shared_ptr<MutableSymbol<T>> op) override { return op; }
    std::shared_ptr<MathOp<T>> visit(std::shared_ptr<ConstantSymbol<T>> op) override { return op; }
    std::shared_ptr<MathOp<T>> visit(std::shared_ptr<OpVariable<T>> op) override { return op; }
    std::shared_ptr<MathOp<T>> visit(std::shared_ptr<ValueVariable<T>> op) override { return op; }
    std::shared_ptr<MathOp<T>> visit(std::shared_ptr<NamedConstant<T>> op) override { return op; }
    std::shared_ptr<MathOp<T>> visit(std::shared_ptr<MutableValue<T>> op) override { return op; }
    std::shared_ptr<MathOp<T>> visit(std::shared_ptr<ConstantValue<T>> op) override { return op; }

    std::shared_ptr<MathOp<T>> visit(std::shared_ptr<Negate<T>> op) override
    {
        return -(op->get_x()->transform(*this));
    }

    std::shared_ptr<MathOp<T>> visit(std::shared_ptr<Sqrt<T>> op) override
    {
        return sqrt(op->get_x()->transform(*this));
    }

    std::shared_ptr<MathOp<T>> visit(std::shared_ptr<Square<T>> op) override
    {
        return square(op->get_x()->transform(*this));
    }

    std::shared_ptr<MathOp<T>> visit(std::shared_ptr<Log<T>> op) override
    {
        return log(op->get_x()->transform(*this));
    }

    std::shared_ptr<MathOp<T>> visit(std::shared_ptr<Sin<T>> op) override
    {
        return sin(op->get_x()->transform(*this));
    }

    std::shared_ptr<MathOp<T>> visit(std::shared_ptr<ASin<T>> op) override
    {
        return asin(op->get_x()->transform(*this));
    }

    std::shared_ptr<MathOp<T>> visit(std::shared_ptr<Cos<T>> op) override
    {
        return cos(op->get_x()->transform(*this));
    }

    std::shared_ptr<MathOp<T>> visit(std::shared_ptr<ACos<T>> op) override
    {
        return acos(op->get_x()->transform(*this));
    }

    std::shared_ptr<MathOp<T>> visit(std::shared_ptr<Tan<T>> op) override
    {
        return tan(op->get_x()->transform(*this));
    }

    std::shared_ptr<MathOp<T>> visit(std::shared_ptr<ATan<T>> op) override
    {
        return atan(op->get_x()->transform(*this));
    }

    std::shared_ptr<MathOp<T>> visit(std::shared_ptr<Pow<T>> op) override
    {
        auto lhs = op->get_lhs()->transform(*this);
        auto rhs = op->get_rhs()->transform(*this);

        if (lhs->is_constant() && lhs->result() == 0)
        {
            return Factory::CreateConstantValue<T>(0.0);
        }

        if (rhs->is_constant() && rhs->result() == 0)
        {
            return Factory::CreateConstantValue<T>(1.0);
        }

        if (rhs->is_constant() && rhs->result() == 1)
        {
            return lhs;
        }

        return lhs ^ rhs;
    }

    std::shared_ptr<MathOp<T>> visit(std::shared_ptr<Mul<T>> op) override
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
            return Factory::CreateConstantValue<T>(0.0);
        }

        return lhs  * rhs;
    }

    std::shared_ptr<MathOp<T>> visit(std::shared_ptr<Div<T>> op) override
    {
        auto lhs = op->get_lhs()->transform(*this);
        auto rhs = op->get_rhs()->transform(*this);

        if (lhs->is_constant() && rhs->is_constant() && lhs->result() == rhs->result())
        {
            return Factory::CreateConstantValue<T>(1.0);
        }

        if (rhs->is_constant() && rhs->result() == 1)
        {
            return lhs;
        }

        if (lhs->is_constant() && lhs->result() == 0)
        {
            return Factory::CreateConstantValue<T>(0.0);
        }

        return lhs / rhs;
    }

    std::shared_ptr<MathOp<T>> visit(std::shared_ptr<Add<T>> op) override
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

    std::shared_ptr<MathOp<T>> visit(std::shared_ptr<Sub<T>> op) override
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

} /* namespace MathOps */

#endif /* REMOVENOOPVISITOR_H */