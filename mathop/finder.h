#ifndef FINDER_H
#define FINDER_H

#include "algeblah.h"

namespace MathOps
{

template <typename T>
struct Finder : public Visitor<T>
{
    Finder(std::shared_ptr<MathOp<T>> target)
        : target(target)
    { }

    virtual VisitorResult<T> visit(std::shared_ptr<Variable<T>> op) override { return op == target ? 1 : 0; }
    virtual VisitorResult<T> visit(std::shared_ptr<ConstantSymbol<T>> op) override { return op == target ? 1 : 0; }
    virtual VisitorResult<T> visit(std::shared_ptr<ValueVariable<T>> op) override { return op == target ? 1 : 0; }
    virtual VisitorResult<T> visit(std::shared_ptr<NamedConstant<T>> op) override { return op == target ? 1 : 0; }
    virtual VisitorResult<T> visit(std::shared_ptr<MutableValue<T>> op) override { return op == target ? 1 : 0; }
    virtual VisitorResult<T> visit(std::shared_ptr<ConstantValue<T>> op) override { return op == target ? 1 : 0; }

    virtual VisitorResult<T> visit(std::shared_ptr<Container<T>> op) override { return count(op); }

    virtual VisitorResult<T> visit(std::shared_ptr<Negate<T>> op) override { return count(op); }
    virtual VisitorResult<T> visit(std::shared_ptr<Sqrt<T>> op) override { return count(op); }
    virtual VisitorResult<T> visit(std::shared_ptr<Log<T>> op) override { return count(op); }
    virtual VisitorResult<T> visit(std::shared_ptr<Log10<T>> op) override { return count(op); }
    virtual VisitorResult<T> visit(std::shared_ptr<Sin<T>> op) override { return count(op); }
    virtual VisitorResult<T> visit(std::shared_ptr<ASin<T>> op) override { return count(op); }
    virtual VisitorResult<T> visit(std::shared_ptr<Cos<T>> op) override { return count(op); }
    virtual VisitorResult<T> visit(std::shared_ptr<ACos<T>> op) override { return count(op); }
    virtual VisitorResult<T> visit(std::shared_ptr<Tan<T>> op) override { return count(op); }
    virtual VisitorResult<T> visit(std::shared_ptr<ATan<T>> op) override { return count(op); }
    virtual VisitorResult<T> visit(std::shared_ptr<Sinh<T>> op) override { return count(op); }
    virtual VisitorResult<T> visit(std::shared_ptr<ASinh<T>> op) override { return count(op); }
    virtual VisitorResult<T> visit(std::shared_ptr<Cosh<T>> op) override { return count(op); }
    virtual VisitorResult<T> visit(std::shared_ptr<ACosh<T>> op) override { return count(op); }
    virtual VisitorResult<T> visit(std::shared_ptr<Tanh<T>> op) override { return count(op); }
    virtual VisitorResult<T> visit(std::shared_ptr<ATanh<T>> op) override { return count(op); }

    virtual VisitorResult<T> visit(std::shared_ptr<Pow<T>> op) override { return count(op); }
    virtual VisitorResult<T> visit(std::shared_ptr<Mul<T>> op) override { return count(op); }
    virtual VisitorResult<T> visit(std::shared_ptr<Div<T>> op) override { return count(op); }
    virtual VisitorResult<T> visit(std::shared_ptr<Add<T>> op) override { return count(op); }
    virtual VisitorResult<T> visit(std::shared_ptr<Sub<T>> op) override { return count(op); }

private:
    const std::shared_ptr<MathOp<T>> target;

    int count(std::shared_ptr<Container<T>> op)
    {
        return (op == target ? 1 : 0) + op->get_inner()->count(*this);
    }

    int count(std::shared_ptr<MathUnaryOp<T>> op)
    {
        return (op == target ? 1 : 0) + op->get_x()->count(*this);
    }

    int count(std::shared_ptr<MathBinaryOp<T>> op)
    {
        return (op == target ? 1 : 0) + op->get_lhs()->count(*this) + op->get_rhs()->count(*this);
    }
};

} /* namespace MathOps */

#endif /* FINDER_H */