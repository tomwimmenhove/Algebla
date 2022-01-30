#ifndef REVERSETRANSFORMER_H
#define REVERSETRANSFORMER_H

#include "algeblah.h"
#include "constants.h"

namespace MathOps
{

template <typename T>
struct ReverseTransformer : public Visitor<T>
{
    ReverseTransformer(std::shared_ptr<MathOp<T>> for_side, std::shared_ptr<MathOp<T>>from)
        : for_side(for_side), from(from)
    { }

    VisitorResult<T> visit(std::shared_ptr<ConstantSymbol<T>>) override { return std::shared_ptr<MathOp<T>>(nullptr); }
    VisitorResult<T> visit(std::shared_ptr<Variable<T>>) override { return std::shared_ptr<MathOp<T>>(nullptr); }
    VisitorResult<T> visit(std::shared_ptr<ValueVariable<T>>) override { return std::shared_ptr<MathOp<T>>(nullptr); }
    VisitorResult<T> visit(std::shared_ptr<NamedConstant<T>>) override { return std::shared_ptr<MathOp<T>>(nullptr); }
    VisitorResult<T> visit(std::shared_ptr<MutableValue<T>>) override { return std::shared_ptr<MathOp<T>>(nullptr); }
    VisitorResult<T> visit(std::shared_ptr<ConstantValue<T>>) override { return std::shared_ptr<MathOp<T>>(nullptr); }

    VisitorResult<T> visit(std::shared_ptr<Container<T>> op) override { return op->get_inner()->transform(*this); }
 
    VisitorResult<T> visit(std::shared_ptr<Negate<T>> op) { return for_side == op->get_x() ? (-from) : nullptr; }

    VisitorResult<T> visit(std::shared_ptr<Sqrt<T>> op)
    {
        return for_side == op->get_x() ? pow<T>(from, ConstantValue<T>::create(2.0)) : nullptr;
    }

    VisitorResult<T> visit(std::shared_ptr<Log<T>> op)
    {
        return for_side == op->get_x() ? pow<T>(Constants::e<T>(), from) : nullptr;
    }

    VisitorResult<T> visit(std::shared_ptr<Log10<T>> op)
    {
        return for_side == op->get_x() ? pow<T>(ConstantValue<T>::create(10.0), from): nullptr;
    }

    VisitorResult<T> visit(std::shared_ptr<Sin<T>> op) { return for_side == op->get_x() ? asin(from) : nullptr; }
    VisitorResult<T> visit(std::shared_ptr<ASin<T>> op) { return for_side == op->get_x() ? sin(from) : nullptr; }
    VisitorResult<T> visit(std::shared_ptr<Cos<T>> op) { return for_side == op->get_x() ? acos(from) : nullptr; }
    VisitorResult<T> visit(std::shared_ptr<ACos<T>> op) { return for_side == op->get_x() ? cos(from) : nullptr; }
    VisitorResult<T> visit(std::shared_ptr<Tan<T>> op) { return for_side == op->get_x() ? atan(from) : nullptr; }
    VisitorResult<T> visit(std::shared_ptr<ATan<T>> op) { return for_side == op->get_x() ? tan(from) : nullptr; }
    VisitorResult<T> visit(std::shared_ptr<Sinh<T>> op) { return for_side == op->get_x() ? asinh(from) : nullptr; }
    VisitorResult<T> visit(std::shared_ptr<ASinh<T>> op) { return for_side == op->get_x() ? sinh(from) : nullptr; }
    VisitorResult<T> visit(std::shared_ptr<Cosh<T>> op) { return for_side == op->get_x() ? acosh(from) : nullptr; }
    VisitorResult<T> visit(std::shared_ptr<ACosh<T>> op) { return for_side == op->get_x() ? cosh(from) : nullptr; }
    VisitorResult<T> visit(std::shared_ptr<Tanh<T>> op) { return for_side == op->get_x() ? atanh(from) : nullptr; }
    VisitorResult<T> visit(std::shared_ptr<ATanh<T>> op) { return for_side == op->get_x() ? tanh(from) : nullptr; }

    VisitorResult<T> visit(std::shared_ptr<Pow<T>> op)
    {
        auto lhs = op->get_lhs();
        auto rhs = op->get_rhs();

        if (for_side == lhs)
        {
            if (rhs->is_constant() && rhs->result() == 2.0)
            {
                return sqrt(from);
            }
            
            return Pow<T>::create(from, (ConstantValue<T>::create(1.0) / rhs));
        }
        else if (for_side == rhs)
        {
            return log(from) / log(lhs);
        }
        else
        {
            return std::shared_ptr<MathOp<T>>(nullptr);
        }
    }

    VisitorResult<T> visit(std::shared_ptr<Mul<T>> op)
    {
        if      (for_side == op->get_lhs()) return from / op->get_rhs();
        else if (for_side == op->get_rhs()) return from / op->get_lhs();
        else                                return std::shared_ptr<MathOp<T>>(nullptr);
    }

    VisitorResult<T> visit(std::shared_ptr<Div<T>> op)
    {
        if      (for_side == op->get_lhs()) return from * op->get_rhs();
        else if (for_side == op->get_rhs()) return op->get_lhs() / from;
        else                                return std::shared_ptr<MathOp<T>>(nullptr);
    }

    VisitorResult<T> visit(std::shared_ptr<Add<T>> op)
    {
        if      (for_side == op->get_lhs()) return from - op->get_rhs();
        else if (for_side == op->get_rhs()) return from - op->get_lhs();
        else                                return std::shared_ptr<MathOp<T>>(nullptr);
    }

    VisitorResult<T> visit(std::shared_ptr<Sub<T>> op)
    {
        if      (for_side == op->get_lhs()) return from + op->get_rhs();
        else if (for_side == op->get_rhs()) return op->get_lhs() - from;
        else                                return std::shared_ptr<MathOp<T>>(nullptr);
    }

private:
    const std::shared_ptr<MathOp<T>> for_side;
    const std::shared_ptr<MathOp<T>> from;
};

} /* namespace MathOps */

#endif /* REVERSETRANSFORMER_H */