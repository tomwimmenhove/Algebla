#ifndef DUMMYTRANSFORMER_H
#define DUMMYTRANSFORMER_H

#include "algeblah.h"

namespace MathOps
{

template <typename T>
struct DummyTransformer : public Visitor<T>
{
    virtual VisitorResult<T> visit(std::shared_ptr<MutableSymbol<T>> op) override { return op; }
    virtual VisitorResult<T> visit(std::shared_ptr<ConstantSymbol<T>> op) override { return op; }
    virtual VisitorResult<T> visit(std::shared_ptr<OpVariable<T>> op) override { return op; }
    virtual VisitorResult<T> visit(std::shared_ptr<ValueVariable<T>> op) override { return op; }
    virtual VisitorResult<T> visit(std::shared_ptr<NamedConstant<T>> op) override { return op; }
    virtual VisitorResult<T> visit(std::shared_ptr<MutableValue<T>> op) override { return op; }
    virtual VisitorResult<T> visit(std::shared_ptr<ConstantValue<T>> op) override { return op; }

    VisitorResult<T> visit(std::shared_ptr<Negate<T>> op) override
    {
        return -(op->get_x()->transform(*this));
    }

    virtual VisitorResult<T> visit(std::shared_ptr<Sqrt<T>> op) override
    {
        return sqrt(op->get_x()->transform(*this));
    }

    virtual VisitorResult<T> visit(std::shared_ptr<Log<T>> op) override
    {
        return log(op->get_x()->transform(*this));
    }

    virtual VisitorResult<T> visit(std::shared_ptr<Sin<T>> op) override
    {
        return sin(op->get_x()->transform(*this));
    }

    virtual VisitorResult<T> visit(std::shared_ptr<ASin<T>> op) override
    {
        return asin(op->get_x()->transform(*this));
    }

    virtual VisitorResult<T> visit(std::shared_ptr<Cos<T>> op) override
    {
        return cos(op->get_x()->transform(*this));
    }

    virtual VisitorResult<T> visit(std::shared_ptr<ACos<T>> op) override
    {
        return acos(op->get_x()->transform(*this));
    }

    virtual VisitorResult<T> visit(std::shared_ptr<Tan<T>> op) override
    {
        return tan(op->get_x()->transform(*this));
    }

    virtual VisitorResult<T> visit(std::shared_ptr<ATan<T>> op) override
    {
        return atan(op->get_x()->transform(*this));
    }

    virtual VisitorResult<T> visit(std::shared_ptr<Pow<T>> op) override
    {
        return Pow<T>::create(op->get_lhs()->transform(*this), op->get_rhs()->transform(*this));
    }

    virtual VisitorResult<T> visit(std::shared_ptr<Mul<T>> op) override
    {
        return op->get_lhs()->transform(*this) * op->get_rhs()->transform(*this);
    }

    virtual VisitorResult<T> visit(std::shared_ptr<Div<T>> op) override
    {
        return op->get_lhs()->transform(*this) / op->get_rhs()->transform(*this);
    }

    virtual VisitorResult<T> visit(std::shared_ptr<Add<T>> op) override
    {
        return op->get_lhs()->transform(*this) + op->get_rhs()->transform(*this);
    }

    virtual VisitorResult<T> visit(std::shared_ptr<Sub<T>> op) override
    {
        return op->get_lhs()->transform(*this) - op->get_rhs()->transform(*this);
    }
};

} /* namespace MathOps */

#endif /* DUMMYTRANSFORMER_H */