#ifndef REVERSETRANSFORMER_H
#define REVERSETRANSFORMER_H

#include "algeblah.h"

namespace MathOps
{

template <typename T>
struct ReverseTransformer : public Transformer<T>
{
    ReverseTransformer(std::shared_ptr<MathOp<T>> for_side, std::shared_ptr<MathOp<T>>from)
        : for_side(for_side), from(from)
    { }

    std::shared_ptr<MathOp<T>> visit(std::shared_ptr<MutableSymbol<T>> op) override { return nullptr; }
    std::shared_ptr<MathOp<T>> visit(std::shared_ptr<ConstantSymbol<T>> op) override { return nullptr; }
    std::shared_ptr<MathOp<T>> visit(std::shared_ptr<OpVariable<T>> op) override { return nullptr; }
    std::shared_ptr<MathOp<T>> visit(std::shared_ptr<ValueVariable<T>> op) override { return nullptr; }
    std::shared_ptr<MathOp<T>> visit(std::shared_ptr<NamedConstant<T>> op) override { return nullptr; }
    std::shared_ptr<MathOp<T>> visit(std::shared_ptr<MutableValue<T>> op) override { return nullptr; }
    std::shared_ptr<MathOp<T>> visit(std::shared_ptr<ConstantValue<T>> op) override { return nullptr; }
 
    std::shared_ptr<MathOp<T>> visit(std::shared_ptr<Negate<T>> op)
    {
        if (for_side != op->get_x()) return nullptr;
        return -from;
    }

    std::shared_ptr<MathOp<T>> visit(std::shared_ptr<Sqrt<T>> op)
    {
        if (for_side != op->get_x()) return nullptr;
        return square(from);
    }

    std::shared_ptr<MathOp<T>> visit(std::shared_ptr<Square<T>> op)
    {
        if (for_side != op->get_x()) return nullptr;
        return sqrt(from);
    }

    std::shared_ptr<MathOp<T>> visit(std::shared_ptr<Log<T>> op)
    {
        if (for_side != op->get_x()) return nullptr;
        return Factory::CreateSymbolE<T>() ^ from;
    }

    std::shared_ptr<MathOp<T>> visit(std::shared_ptr<Sin<T>> op)
    {
        if (for_side != op->get_x()) return nullptr;
        return asin(from);
    }

    std::shared_ptr<MathOp<T>> visit(std::shared_ptr<ASin<T>> op)
    {
        if (for_side != op->get_x()) return nullptr;
        return sin(from);
    }

    std::shared_ptr<MathOp<T>> visit(std::shared_ptr<Cos<T>> op)
    {
        if (for_side != op->get_x()) return nullptr;
        return acos(from);
    }

    std::shared_ptr<MathOp<T>> visit(std::shared_ptr<ACos<T>> op)
    {
        if (for_side != op->get_x()) return nullptr;
        return cos(from);
    }

    std::shared_ptr<MathOp<T>> visit(std::shared_ptr<Tan<T>> op)
    {
        if (for_side != op->get_x()) return nullptr;
        return atan(from);
    }

    std::shared_ptr<MathOp<T>> visit(std::shared_ptr<ATan<T>> op)
    {
        if (for_side != op->get_x()) return nullptr;
        return tan(from);
    }

    std::shared_ptr<MathOp<T>> visit(std::shared_ptr<Pow<T>> op)
    {
        auto lhs = op->get_lhs();
        auto rhs = op->get_rhs();

        if (for_side == lhs)
        {
            if (rhs->is_constant() && rhs->result() == 2.0)
            {
                return sqrt(from);
            }
            
            return from ^ (Factory::CreateConstantValue<T>(1.0) / rhs);
        }
        else if (for_side == rhs)
        {
            return log(from) / log(lhs);
        }
        else
        {
            return nullptr;
        }
    }

    std::shared_ptr<MathOp<T>> visit(std::shared_ptr<Mul<T>> op)
    {
        if      (for_side == op->get_lhs()) return from / op->get_rhs();
        else if (for_side == op->get_rhs()) return from / op->get_lhs();
        else                                return nullptr;
    }

    std::shared_ptr<MathOp<T>> visit(std::shared_ptr<Div<T>> op)
    {
        if      (for_side == op->get_lhs()) return from * op->get_rhs();
        else if (for_side == op->get_rhs()) return op->get_lhs() / from;
        else                                return nullptr;
    }

    std::shared_ptr<MathOp<T>> visit(std::shared_ptr<Add<T>> op)
    {
        if      (for_side == op->get_lhs()) return from - op->get_rhs();
        else if (for_side == op->get_rhs()) return from - op->get_lhs();
        else                                return nullptr;
    }

    std::shared_ptr<MathOp<T>> visit(std::shared_ptr<Sub<T>> op)
    {
        if      (for_side == op->get_lhs()) return from + op->get_rhs();
        else if (for_side == op->get_rhs()) return op->get_lhs() - from;
        else                                return nullptr;
    }

private:
    const std::shared_ptr<MathOp<T>> for_side;
    const std::shared_ptr<MathOp<T>> from;
};

} /* namespace MathOps */

#endif /* REVERSETRANSFORMER_H */