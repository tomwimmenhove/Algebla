#ifndef REVERSETRANSFORMER_H
#define REVERSETRANSFORMER_H

#include "algeblah.h"

namespace MathOps
{

template <typename T>
struct MathOpReverseTransformer : public MathOpTransformer<T>
{
    MathOpReverseTransformer(std::shared_ptr<MathOp<T>> for_side, std::shared_ptr<MathOp<T>>from)
        : for_side(for_side), from(from)
    { }

    std::shared_ptr<MathOp<T>> visit(std::shared_ptr<MathOpMutableSymbol<T>> op) override { return nullptr; }
    std::shared_ptr<MathOp<T>> visit(std::shared_ptr<MathOpConstantSymbol<T>> op) override { return nullptr; }
    std::shared_ptr<MathOp<T>> visit(std::shared_ptr<MathOpVariable<T>> op) override { return nullptr; }
    std::shared_ptr<MathOp<T>> visit(std::shared_ptr<MathOpValueVariable<T>> op) override { return nullptr; }
    std::shared_ptr<MathOp<T>> visit(std::shared_ptr<MathOpNamedConstant<T>> op) override { return nullptr; }
    std::shared_ptr<MathOp<T>> visit(std::shared_ptr<MathOpMutableValue<T>> op) override { return nullptr; }
    std::shared_ptr<MathOp<T>> visit(std::shared_ptr<MathOpConstantValue<T>> op) override { return nullptr; }
 
    std::shared_ptr<MathOp<T>> visit(std::shared_ptr<MathOpNegate<T>> op)
    {
        if (for_side != op->get_x()) return nullptr;
        return -from;
    }

    std::shared_ptr<MathOp<T>> visit(std::shared_ptr<MathOpSqrt<T>> op)
    {
        if (for_side != op->get_x()) return nullptr;
        return square(from);
    }

    std::shared_ptr<MathOp<T>> visit(std::shared_ptr<MathOpSquare<T>> op)
    {
        if (for_side != op->get_x()) return nullptr;
        return sqrt(from);
    }

    std::shared_ptr<MathOp<T>> visit(std::shared_ptr<MathOpLog<T>> op)
    {
        if (for_side != op->get_x()) return nullptr;
        return MathFactory::SymbolE<T>() ^ from;
    }

    std::shared_ptr<MathOp<T>> visit(std::shared_ptr<MathOpSin<T>> op)
    {
        if (for_side != op->get_x()) return nullptr;
        return asin(from);
    }

    std::shared_ptr<MathOp<T>> visit(std::shared_ptr<MathOpASin<T>> op)
    {
        if (for_side != op->get_x()) return nullptr;
        return sin(from);
    }

    std::shared_ptr<MathOp<T>> visit(std::shared_ptr<MathOpCos<T>> op)
    {
        if (for_side != op->get_x()) return nullptr;
        return acos(from);
    }

    std::shared_ptr<MathOp<T>> visit(std::shared_ptr<MathOpACos<T>> op)
    {
        if (for_side != op->get_x()) return nullptr;
        return cos(from);
    }

    std::shared_ptr<MathOp<T>> visit(std::shared_ptr<MathOpTan<T>> op)
    {
        if (for_side != op->get_x()) return nullptr;
        return atan(from);
    }

    std::shared_ptr<MathOp<T>> visit(std::shared_ptr<MathOpATan<T>> op)
    {
        if (for_side != op->get_x()) return nullptr;
        return tan(from);
    }

    std::shared_ptr<MathOp<T>> visit(std::shared_ptr<MathOpPow<T>> op)
    {
        auto lhs = op->get_lhs();
        auto rhs = op->get_rhs();

        if (for_side == lhs)
        {
            if (rhs->is_constant() && rhs->result() == 2.0)
            {
                return sqrt(from);
            }
            
            return from ^ (MathFactory::ConstantValue<T>(1.0) / rhs);
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

    std::shared_ptr<MathOp<T>> visit(std::shared_ptr<MathOpMul<T>> op)
    {
        if      (for_side == op->get_lhs()) return from / op->get_rhs();
        else if (for_side == op->get_rhs()) return from / op->get_lhs();
        else                                return nullptr;
    }

    std::shared_ptr<MathOp<T>> visit(std::shared_ptr<MathOpDiv<T>> op)
    {
        if      (for_side == op->get_lhs()) return from * op->get_rhs();
        else if (for_side == op->get_rhs()) return op->get_lhs() / from;
        else                                return nullptr;
    }

    std::shared_ptr<MathOp<T>> visit(std::shared_ptr<MathOpAdd<T>> op)
    {
        if      (for_side == op->get_lhs()) return from - op->get_rhs();
        else if (for_side == op->get_rhs()) return from - op->get_lhs();
        else                                return nullptr;
    }

    std::shared_ptr<MathOp<T>> visit(std::shared_ptr<MathOpSub<T>> op)
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