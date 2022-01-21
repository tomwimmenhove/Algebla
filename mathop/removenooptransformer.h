#ifndef REMOVENOOPVISITOR_H
#define REMOVENOOPVISITOR_H

#include "dummytransformer.h"

namespace MathOps
{

template <typename T>
struct MathOpRemoveNoOpTransformer : public DummyTransformer<T>
{
    VisitorResult<T> visit(std::shared_ptr<Pow<T>> op) override
    {
        auto lhs = op->get_lhs()->transform(*this);
        auto rhs = op->get_rhs()->transform(*this);

        if (lhs->is_constant() && lhs->result() == 0)
        {
            return ConstantValue<T>::create(0.0);
        }

        if (rhs->is_constant() && rhs->result() == 0)
        {
            return ConstantValue<T>::create(1.0);
        }

        if (rhs->is_constant() && rhs->result() == 1)
        {
            return lhs;
        }

        return Pow<T>::create(lhs, rhs);
    }

    VisitorResult<T> visit(std::shared_ptr<Mul<T>> op) override
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
            return ConstantValue<T>::create(0.0);
        }

        return lhs  * rhs;
    }

    VisitorResult<T> visit(std::shared_ptr<Div<T>> op) override
    {
        auto lhs = op->get_lhs()->transform(*this);
        auto rhs = op->get_rhs()->transform(*this);

        if (lhs->is_constant() && rhs->is_constant() && lhs->result() == rhs->result())
        {
            return ConstantValue<T>::create(1.0);
        }

        if (rhs->is_constant() && rhs->result() == 1)
        {
            return lhs;
        }

        if (lhs->is_constant() && lhs->result() == 0)
        {
            return ConstantValue<T>::create(0.0);
        }

        return lhs / rhs;
    }

    VisitorResult<T> visit(std::shared_ptr<Add<T>> op) override
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

    VisitorResult<T> visit(std::shared_ptr<Sub<T>> op) override
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