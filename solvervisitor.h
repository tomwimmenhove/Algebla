#ifndef SOLVERVISITOR_H
#define SOLVERVISITOR_H

#include "algeblah.h"

template <typename T>
struct MathOpSolverVisitor : public MathOpVisitor<T>
{
    MathOpSolverVisitor(std::shared_ptr<MathOp<T>> solve_for, std::shared_ptr<MathOp<T>> from_result)
        : solve_for(solve_for), from_result(from_result)
    { }

    virtual std::shared_ptr<MathOp<T>> visit(std::shared_ptr<MathOpMutableSymbol<T>> op) override { return op == solve_for ? from_result : nullptr; }
    virtual std::shared_ptr<MathOp<T>> visit(std::shared_ptr<MathOpConstantSymbol<T>> op) override { return op == solve_for ? from_result : nullptr; }
    virtual std::shared_ptr<MathOp<T>> visit(std::shared_ptr<MathOpVariable<T>> op) override { return op == solve_for ? from_result : nullptr; }
    virtual std::shared_ptr<MathOp<T>> visit(std::shared_ptr<MathOpMutableValue<T>> op) override { return op == solve_for ? from_result : nullptr; }
    virtual std::shared_ptr<MathOp<T>> visit(std::shared_ptr<MathOpConstantValue<T>> op) override { return op == solve_for ? from_result : nullptr; }

    virtual std::shared_ptr<MathOp<T>> visit(std::shared_ptr<MathOpSqrt<T>> op) override { return solve_for_unary(op, op->get_x()); }
    virtual std::shared_ptr<MathOp<T>> visit(std::shared_ptr<MathOpSquare<T>> op) override { return solve_for_unary(op, op->get_x()); }
    virtual std::shared_ptr<MathOp<T>> visit(std::shared_ptr<MathOpLog<T>> op) override { return solve_for_unary(op, op->get_x()); }
    virtual std::shared_ptr<MathOp<T>> visit(std::shared_ptr<MathOpSin<T>> op) override { return solve_for_unary(op, op->get_x()); }
    virtual std::shared_ptr<MathOp<T>> visit(std::shared_ptr<MathOpASin<T>> op) override { return solve_for_unary(op, op->get_x()); }
    virtual std::shared_ptr<MathOp<T>> visit(std::shared_ptr<MathOpCos<T>> op) override { return solve_for_unary(op, op->get_x()); }
    virtual std::shared_ptr<MathOp<T>> visit(std::shared_ptr<MathOpACos<T>> op) override { return solve_for_unary(op, op->get_x()); }
    virtual std::shared_ptr<MathOp<T>> visit(std::shared_ptr<MathOpTan<T>> op) override { return solve_for_unary(op, op->get_x()); }
    virtual std::shared_ptr<MathOp<T>> visit(std::shared_ptr<MathOpATan<T>> op) override { return solve_for_unary(op, op->get_x()); }

    virtual std::shared_ptr<MathOp<T>> visit(std::shared_ptr<MathOpPow<T>> op) override { return solve_for_binary(op, op->get_lhs(), op->get_rhs()); }
    virtual std::shared_ptr<MathOp<T>> visit(std::shared_ptr<MathOpMul<T>> op) override { return solve_for_binary(op, op->get_lhs(), op->get_rhs()); }
    virtual std::shared_ptr<MathOp<T>> visit(std::shared_ptr<MathOpDiv<T>> op) override { return solve_for_binary(op, op->get_lhs(), op->get_rhs()); }
    virtual std::shared_ptr<MathOp<T>> visit(std::shared_ptr<MathOpAdd<T>> op) override { return solve_for_binary(op, op->get_lhs(), op->get_rhs()); }
    virtual std::shared_ptr<MathOp<T>> visit(std::shared_ptr<MathOpSub<T>> op) override { return solve_for_binary(op, op->get_lhs(), op->get_rhs()); }

private:
    std::shared_ptr<MathOp<T>> solve_for;
    std::shared_ptr<MathOp<T>> from_result;

    std::shared_ptr<MathOp<T>>solve_for_unary(std::shared_ptr<MathOp<T>> op, std::shared_ptr<MathOp<T>> x)
    {
        auto from_x = op->rearranged(x, from_result);
        return x->accept(new MathOpSolverVisitor<T>(solve_for, from_x));
    }

    std::shared_ptr<MathOp<T>>solve_for_binary(std::shared_ptr<MathOp<T>> op,
        std::shared_ptr<MathOp<T>> lhs, std::shared_ptr<MathOp<T>> rhs)
    {
        auto from_lhs = op->rearranged(lhs, from_result);

        auto solved_lhs = lhs->accept(new MathOpSolverVisitor<T>(solve_for, from_lhs));
        if (solved_lhs != nullptr)
        {
            return solved_lhs;
        }

        auto from_rhs = op->rearranged(rhs, from_result);

        return rhs->accept(new MathOpSolverVisitor<T>(solve_for, from_rhs));




        // auto from_lhs = op->rearranged(lhs, from_result);

        // auto solved_lhs = lhs->accept(new MathOpSolverVisitor<T>(solve_for, from_lhs));
        // if (solved_lhs != nullptr)
        // {
        //     return solved_lhs;
        // }

        // auto from_rhs = op->rearranged(rhs, from_result);

        // return rhs->accept(new MathOpSolverVisitor<T>(solve_for, from_rhs));
    }
};

#endif /* SOLVERVISITOR_H */