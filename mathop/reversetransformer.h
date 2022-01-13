#ifndef REVERSETRANSFORMER_H
#define REVERSETRANSFORMER_H

#include "algeblah.h"

namespace MathOps
{

template <typename T>
struct ReverseTransformer : public Visitor<T>
{
    ReverseTransformer(std::shared_ptr<MathOp<T>> for_side, std::shared_ptr<MathOp<T>>from)
        : for_side(for_side), from(from)
    { }

    VisitorResult<T> visit(std::shared_ptr<ConstantSymbol<T>> op) override { return std::shared_ptr<MathOp<T>>(nullptr); }
    VisitorResult<T> visit(std::shared_ptr<Variable<T>> op) override { return std::shared_ptr<MathOp<T>>(nullptr); }
    VisitorResult<T> visit(std::shared_ptr<ValueVariable<T>> op) override { return std::shared_ptr<MathOp<T>>(nullptr); }
    VisitorResult<T> visit(std::shared_ptr<NamedConstant<T>> op) override { return std::shared_ptr<MathOp<T>>(nullptr); }
    VisitorResult<T> visit(std::shared_ptr<MutableValue<T>> op) override { return std::shared_ptr<MathOp<T>>(nullptr); }
    VisitorResult<T> visit(std::shared_ptr<ConstantValue<T>> op) override { return std::shared_ptr<MathOp<T>>(nullptr); }
 
    VisitorResult<T> visit(std::shared_ptr<Negate<T>> op)
    {
        if (for_side != op->get_x()) return std::shared_ptr<MathOp<T>>(nullptr);
        return -from;
    }

    VisitorResult<T> visit(std::shared_ptr<Sqrt<T>> op)
    {
        if (for_side != op->get_x()) return std::shared_ptr<MathOp<T>>(nullptr);
        return Pow<T>::create(from, Factory::CreateConstantValue<T>(2.0));
    }

    VisitorResult<T> visit(std::shared_ptr<Log<T>> op)
    {
        if (for_side != op->get_x()) return std::shared_ptr<MathOp<T>>(nullptr);
        return Pow<T>::create(Factory::CreateSymbolE<T>(), from);
    }

    VisitorResult<T> visit(std::shared_ptr<Sin<T>> op)
    {
        if (for_side != op->get_x()) return std::shared_ptr<MathOp<T>>(nullptr);
        return asin(from);
    }

    VisitorResult<T> visit(std::shared_ptr<ASin<T>> op)
    {
        if (for_side != op->get_x()) return std::shared_ptr<MathOp<T>>(nullptr);
        return sin(from);
    }

    VisitorResult<T> visit(std::shared_ptr<Cos<T>> op)
    {
        if (for_side != op->get_x()) return std::shared_ptr<MathOp<T>>(nullptr);
        return acos(from);
    }

    VisitorResult<T> visit(std::shared_ptr<ACos<T>> op)
    {
        if (for_side != op->get_x()) return std::shared_ptr<MathOp<T>>(nullptr);
        return cos(from);
    }

    VisitorResult<T> visit(std::shared_ptr<Tan<T>> op)
    {
        if (for_side != op->get_x()) return std::shared_ptr<MathOp<T>>(nullptr);
        return atan(from);
    }

    VisitorResult<T> visit(std::shared_ptr<ATan<T>> op)
    {
        if (for_side != op->get_x()) return std::shared_ptr<MathOp<T>>(nullptr);
        return tan(from);
    }

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
            
            return Pow<T>::create(from, (Factory::CreateConstantValue<T>(1.0) / rhs));
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