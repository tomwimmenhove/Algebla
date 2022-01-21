#ifndef DUMMYTRANSFORMER_H
#define DUMMYTRANSFORMER_H

#include "algeblah.h"

namespace MathOps
{


/* XXX: NOTE: THIS TRANSFORMER DOES NOT WALK INTO CONTAINERS!
 *      Our options are:
 *       - to simply return the original container, which will stop us from walking further down the tree <= this is what is done
 *       - to return the inner MathOp that the container holds, effectively expanding all containers
 *       - to create a new container which the same name, which could break references
 *       - to set a new internal MathOp in the existing Container, but we should not alter the original tree
 */
template <typename T>
struct DummyTransformer : public Visitor<T>
{
    virtual VisitorResult<T> visit(std::shared_ptr<ConstantSymbol<T>> op) override { return op; }
    virtual VisitorResult<T> visit(std::shared_ptr<Variable<T>> op) override { return op; }
    virtual VisitorResult<T> visit(std::shared_ptr<ValueVariable<T>> op) override { return op; }
    virtual VisitorResult<T> visit(std::shared_ptr<NamedConstant<T>> op) override { return op; }
    virtual VisitorResult<T> visit(std::shared_ptr<MutableValue<T>> op) override { return op; }
    virtual VisitorResult<T> visit(std::shared_ptr<ConstantValue<T>> op) override { return op; }

    virtual VisitorResult<T> visit(std::shared_ptr<Container<T>> op) override { return op; }
    // {
    //     return op->get_inner()->transform(*this);
    // }

    virtual VisitorResult<T> visit(std::shared_ptr<Negate<T>> op) override
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

    virtual VisitorResult<T> visit(std::shared_ptr<Log10<T>> op) override
    {
        return log10(op->get_x()->transform(*this));
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

    virtual VisitorResult<T> visit(std::shared_ptr<Sinh<T>> op) override
    {
        return sinh(op->get_x()->transform(*this));
    }

    virtual VisitorResult<T> visit(std::shared_ptr<ASinh<T>> op) override
    {
        return asinh(op->get_x()->transform(*this));
    }

    virtual VisitorResult<T> visit(std::shared_ptr<Cosh<T>> op) override
    {
        return cosh(op->get_x()->transform(*this));
    }

    virtual VisitorResult<T> visit(std::shared_ptr<ACosh<T>> op) override
    {
        return acosh(op->get_x()->transform(*this));
    }

    virtual VisitorResult<T> visit(std::shared_ptr<Tanh<T>> op) override
    {
        return tanh(op->get_x()->transform(*this));
    }

    virtual VisitorResult<T> visit(std::shared_ptr<ATanh<T>> op) override
    {
        return atanh(op->get_x()->transform(*this));
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