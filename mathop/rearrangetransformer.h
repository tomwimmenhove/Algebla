#ifndef REARRANGETRANSFORMER_H
#define REARRANGETRANSFORMER_H

#include "algeblah.h"
#include "reversetransformer.h"

namespace MathOps
{

template <typename T>
struct MathOpRearrangeTransformer : public MathOpTransformer<T>
{
    MathOpRearrangeTransformer(std::shared_ptr<MathOp<T>> solve_for, std::shared_ptr<MathOp<T>> from)
        : solve_for(solve_for), from(from)
    { }

    std::shared_ptr<MathOp<T>> visit(std::shared_ptr<MathOpMutableSymbol<T>> op) override { return op == solve_for ? from : nullptr; }
    std::shared_ptr<MathOp<T>> visit(std::shared_ptr<MathOpConstantSymbol<T>> op) override { return op == solve_for ? from : nullptr; }
    std::shared_ptr<MathOp<T>> visit(std::shared_ptr<MathOpVariable<T>> op) override { return op == solve_for ? from : nullptr; }
    std::shared_ptr<MathOp<T>> visit(std::shared_ptr<MathOpValueVariable<T>> op) override { return op == solve_for ? from : nullptr; }
    std::shared_ptr<MathOp<T>> visit(std::shared_ptr<MathOpNamedConstant<T>> op) override { return op == solve_for ? from : nullptr; }
    std::shared_ptr<MathOp<T>> visit(std::shared_ptr<MathOpMutableValue<T>> op) override { return op == solve_for ? from : nullptr; }
    std::shared_ptr<MathOp<T>> visit(std::shared_ptr<MathOpConstantValue<T>> op) override { return op == solve_for ? from : nullptr; }

    std::shared_ptr<MathOp<T>> visit(std::shared_ptr<MathOpNegate<T>> op) override { return solve_for_unary(op, op->get_x()); }
    std::shared_ptr<MathOp<T>> visit(std::shared_ptr<MathOpSqrt<T>> op) override { return solve_for_unary(op, op->get_x()); }
    std::shared_ptr<MathOp<T>> visit(std::shared_ptr<MathOpSquare<T>> op) override { return solve_for_unary(op, op->get_x()); }
    std::shared_ptr<MathOp<T>> visit(std::shared_ptr<MathOpLog<T>> op) override { return solve_for_unary(op, op->get_x()); }
    std::shared_ptr<MathOp<T>> visit(std::shared_ptr<MathOpSin<T>> op) override { return solve_for_unary(op, op->get_x()); }
    std::shared_ptr<MathOp<T>> visit(std::shared_ptr<MathOpASin<T>> op) override { return solve_for_unary(op, op->get_x()); }
    std::shared_ptr<MathOp<T>> visit(std::shared_ptr<MathOpCos<T>> op) override { return solve_for_unary(op, op->get_x()); }
    std::shared_ptr<MathOp<T>> visit(std::shared_ptr<MathOpACos<T>> op) override { return solve_for_unary(op, op->get_x()); }
    std::shared_ptr<MathOp<T>> visit(std::shared_ptr<MathOpTan<T>> op) override { return solve_for_unary(op, op->get_x()); }
    std::shared_ptr<MathOp<T>> visit(std::shared_ptr<MathOpATan<T>> op) override { return solve_for_unary(op, op->get_x()); }

    std::shared_ptr<MathOp<T>> visit(std::shared_ptr<MathOpPow<T>> op) override { return solve_for_binary(op, op->get_lhs(), op->get_rhs()); }
    std::shared_ptr<MathOp<T>> visit(std::shared_ptr<MathOpMul<T>> op) override { return solve_for_binary(op, op->get_lhs(), op->get_rhs()); }
    std::shared_ptr<MathOp<T>> visit(std::shared_ptr<MathOpDiv<T>> op) override { return solve_for_binary(op, op->get_lhs(), op->get_rhs()); }
    std::shared_ptr<MathOp<T>> visit(std::shared_ptr<MathOpAdd<T>> op) override { return solve_for_binary(op, op->get_lhs(), op->get_rhs()); }
    std::shared_ptr<MathOp<T>> visit(std::shared_ptr<MathOpSub<T>> op) override { return solve_for_binary(op, op->get_lhs(), op->get_rhs()); }

private:
    const std::shared_ptr<MathOp<T>> solve_for;
    const std::shared_ptr<MathOp<T>> from;

    std::shared_ptr<MathOp<T>>solve_for_unary(std::shared_ptr<MathOp<T>> op, std::shared_ptr<MathOp<T>> x)
    {
        auto from_x = op->transform(MathOpReverseTransformer<T>(x, from));
        auto result = x->transform(MathOpRearrangeTransformer<T>(solve_for, from_x));

        return result;
    }

    std::shared_ptr<MathOp<T>>solve_for_binary(std::shared_ptr<MathOp<T>> op,
        std::shared_ptr<MathOp<T>> lhs, std::shared_ptr<MathOp<T>> rhs)
    {
        auto from_lhs = op->transform(MathOpReverseTransformer<T>(lhs, from));
        auto solved_lhs = lhs->transform(MathOpRearrangeTransformer<T>(solve_for, from_lhs));

        if (solved_lhs != nullptr)
        {
            return solved_lhs;
        }

        auto from_rhs = op->transform(MathOpReverseTransformer<T>(rhs, from));
        auto solved_rhs = rhs->transform(MathOpRearrangeTransformer<T>(solve_for, from_rhs));

        return solved_rhs;
    }
};

} /* namespace MathOps */

#endif /* REARRANGETRANSFORMER_H */