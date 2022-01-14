#ifndef REARRANGETRANSFORMER_H
#define REARRANGETRANSFORMER_H

#include "algeblah.h"
#include "reversetransformer.h"

namespace MathOps
{

template <typename T>
struct MathOpRearrangeTransformer : public Visitor<T>
{
    MathOpRearrangeTransformer(std::shared_ptr<MathOp<T>> solve_for, std::shared_ptr<MathOp<T>> from)
        : solve_for(solve_for), from(from)
    { }

    VisitorResult<T> visit(std::shared_ptr<ConstantSymbol<T>> op) override { return op == solve_for ? from : nullptr; }
    VisitorResult<T> visit(std::shared_ptr<Variable<T>> op) override { return op == solve_for ? from : nullptr; }
    VisitorResult<T> visit(std::shared_ptr<ValueVariable<T>> op) override { return op == solve_for ? from : nullptr; }
    VisitorResult<T> visit(std::shared_ptr<NamedConstant<T>> op) override { return op == solve_for ? from : nullptr; }
    VisitorResult<T> visit(std::shared_ptr<MutableValue<T>> op) override { return op == solve_for ? from : nullptr; }
    VisitorResult<T> visit(std::shared_ptr<ConstantValue<T>> op) override { return op == solve_for ? from : nullptr; }

    VisitorResult<T> visit(std::shared_ptr<Negate<T>> op) override { return solve_for_unary(op, op->get_x()); }
    VisitorResult<T> visit(std::shared_ptr<Sqrt<T>> op) override { return solve_for_unary(op, op->get_x()); }
    VisitorResult<T> visit(std::shared_ptr<Log<T>> op) override { return solve_for_unary(op, op->get_x()); }
    VisitorResult<T> visit(std::shared_ptr<Log10<T>> op) override { return solve_for_unary(op, op->get_x()); }
    VisitorResult<T> visit(std::shared_ptr<Sin<T>> op) override { return solve_for_unary(op, op->get_x()); }
    VisitorResult<T> visit(std::shared_ptr<ASin<T>> op) override { return solve_for_unary(op, op->get_x()); }
    VisitorResult<T> visit(std::shared_ptr<Cos<T>> op) override { return solve_for_unary(op, op->get_x()); }
    VisitorResult<T> visit(std::shared_ptr<ACos<T>> op) override { return solve_for_unary(op, op->get_x()); }
    VisitorResult<T> visit(std::shared_ptr<Tan<T>> op) override { return solve_for_unary(op, op->get_x()); }
    VisitorResult<T> visit(std::shared_ptr<ATan<T>> op) override { return solve_for_unary(op, op->get_x()); }
    VisitorResult<T> visit(std::shared_ptr<Sinh<T>> op) override { return solve_for_unary(op, op->get_x()); }
    VisitorResult<T> visit(std::shared_ptr<ASinh<T>> op) override { return solve_for_unary(op, op->get_x()); }
    VisitorResult<T> visit(std::shared_ptr<Cosh<T>> op) override { return solve_for_unary(op, op->get_x()); }
    VisitorResult<T> visit(std::shared_ptr<ACosh<T>> op) override { return solve_for_unary(op, op->get_x()); }
    VisitorResult<T> visit(std::shared_ptr<Tanh<T>> op) override { return solve_for_unary(op, op->get_x()); }
    VisitorResult<T> visit(std::shared_ptr<ATanh<T>> op) override { return solve_for_unary(op, op->get_x()); }

    VisitorResult<T> visit(std::shared_ptr<Pow<T>> op) override { return solve_for_binary(op, op->get_lhs(), op->get_rhs()); }
    VisitorResult<T> visit(std::shared_ptr<Mul<T>> op) override { return solve_for_binary(op, op->get_lhs(), op->get_rhs()); }
    VisitorResult<T> visit(std::shared_ptr<Div<T>> op) override { return solve_for_binary(op, op->get_lhs(), op->get_rhs()); }
    VisitorResult<T> visit(std::shared_ptr<Add<T>> op) override { return solve_for_binary(op, op->get_lhs(), op->get_rhs()); }
    VisitorResult<T> visit(std::shared_ptr<Sub<T>> op) override { return solve_for_binary(op, op->get_lhs(), op->get_rhs()); }

private:
    const std::shared_ptr<MathOp<T>> solve_for;
    const std::shared_ptr<MathOp<T>> from;

    std::shared_ptr<MathOp<T>>solve_for_unary(std::shared_ptr<MathOp<T>> op, std::shared_ptr<MathOp<T>> x)
    {
        auto from_x = op->transform(ReverseTransformer<T>(x, from));
        auto result = x->transform(MathOpRearrangeTransformer<T>(solve_for, from_x));

        return result;
    }

    std::shared_ptr<MathOp<T>>solve_for_binary(std::shared_ptr<MathOp<T>> op,
        std::shared_ptr<MathOp<T>> lhs, std::shared_ptr<MathOp<T>> rhs)
    {
        auto from_lhs = op->transform(ReverseTransformer<T>(lhs, from));
        auto solved_lhs = lhs->transform(MathOpRearrangeTransformer<T>(solve_for, from_lhs));

        if (solved_lhs != nullptr)
        {
            return solved_lhs;
        }

        auto from_rhs = op->transform(ReverseTransformer<T>(rhs, from));
        auto solved_rhs = rhs->transform(MathOpRearrangeTransformer<T>(solve_for, from_rhs));

        return solved_rhs;
    }
};

} /* namespace MathOps */

#endif /* REARRANGETRANSFORMER_H */