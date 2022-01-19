#ifndef FINDNAMEDVALUETRANSFORMER_H
#define FINDNAMEDVALUETRANSFORMER_H

#include "algeblah.h"

namespace MathOps
{

template <typename T>
struct FindNamedValueTransformer : public Visitor<T>
{
    FindNamedValueTransformer(std::string symbol)
        : symbol(symbol)
    { }

    VisitorResult<T> visit(std::shared_ptr<ConstantSymbol<T>> op) override { return std::shared_ptr<MathOp<T>>(nullptr); }
    VisitorResult<T> visit(std::shared_ptr<MutableValue<T>> op) override { return std::shared_ptr<MathOp<T>>(nullptr); }
    VisitorResult<T> visit(std::shared_ptr<ConstantValue<T>> op) override { return std::shared_ptr<MathOp<T>>(nullptr); }

    VisitorResult<T> visit(std::shared_ptr<Variable<T>> op) override
    {
        return op->get_symbol() == symbol ? op : nullptr;
    }

    VisitorResult<T> visit(std::shared_ptr<ValueVariable<T>> op) override
    {
        return op->get_symbol() == symbol ? op : nullptr;
    }

    VisitorResult<T> visit(std::shared_ptr<NamedConstant<T>> op) override
    {
        return op->get_symbol() == symbol ? op : nullptr;
    }

    VisitorResult<T> visit(std::shared_ptr<Container<T>> op) override
    {
        return op->get_external()->transform(*this);
    }

    VisitorResult<T> visit(std::shared_ptr<Sqrt<T>> op) override
    {
        return op->get_x()->transform(*this);
    }

    VisitorResult<T> visit(std::shared_ptr<Negate<T>> op) override
    {
        return op->get_x()->transform(*this);
    }

    VisitorResult<T> visit(std::shared_ptr<Log<T>> op) override
    {
        return op->get_x()->transform(*this);
    }

    VisitorResult<T> visit(std::shared_ptr<Log10<T>> op) override
    {
        return op->get_x()->transform(*this);
    }

    VisitorResult<T> visit(std::shared_ptr<Sin<T>> op) override
    {
        return op->get_x()->transform(*this);
    }

    VisitorResult<T> visit(std::shared_ptr<ASin<T>> op) override
    {
        return op->get_x()->transform(*this);
    }

    VisitorResult<T> visit(std::shared_ptr<Cos<T>> op) override
    {
        return op->get_x()->transform(*this);
    }

    VisitorResult<T> visit(std::shared_ptr<ACos<T>> op) override
    {
        return op->get_x()->transform(*this);
    }

    VisitorResult<T> visit(std::shared_ptr<Tan<T>> op) override
    {
        return op->get_x()->transform(*this);
    }

    VisitorResult<T> visit(std::shared_ptr<ATan<T>> op) override
    {
        return op->get_x()->transform(*this);
    }

    VisitorResult<T> visit(std::shared_ptr<Sinh<T>> op) override
    {
        return op->get_x()->transform(*this);
    }

    VisitorResult<T> visit(std::shared_ptr<ASinh<T>> op) override
    {
        return op->get_x()->transform(*this);
    }

    VisitorResult<T> visit(std::shared_ptr<Cosh<T>> op) override
    {
        return op->get_x()->transform(*this);
    }

    VisitorResult<T> visit(std::shared_ptr<ACosh<T>> op) override
    {
        return op->get_x()->transform(*this);
    }

    VisitorResult<T> visit(std::shared_ptr<Tanh<T>> op) override
    {
        return op->get_x()->transform(*this);
    }

    VisitorResult<T> visit(std::shared_ptr<ATanh<T>> op) override
    {
        return op->get_x()->transform(*this);
    }

    VisitorResult<T> visit(std::shared_ptr<Pow<T>> op) override
    {
        auto lhs = op->get_lhs()->transform(*this);
        return lhs ? lhs : op->get_rhs()->transform(*this);
    }

    VisitorResult<T> visit(std::shared_ptr<Mul<T>> op) override
    {
        auto lhs = op->get_lhs()->transform(*this);
        return lhs ? lhs : op->get_rhs()->transform(*this);
    }

    VisitorResult<T> visit(std::shared_ptr<Div<T>> op) override
    {
        auto lhs = op->get_lhs()->transform(*this);
        return lhs ? lhs : op->get_rhs()->transform(*this);
    }

    VisitorResult<T> visit(std::shared_ptr<Add<T>> op) override
    {
        auto lhs = op->get_lhs()->transform(*this);
        return lhs ? lhs : op->get_rhs()->transform(*this);
    }

    VisitorResult<T> visit(std::shared_ptr<Sub<T>> op) override
    {
        auto lhs = op->get_lhs()->transform(*this);
        return lhs ? lhs : op->get_rhs()->transform(*this);
    }

private:
    std::string symbol;
};

} /* namespace MathOps */

#endif /* FINDNAMEDVALUETRANSFORMER_H */