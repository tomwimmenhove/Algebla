#ifndef REARRANGEMULTITRANSFORMER_H
#define REARRANGEMULTITRANSFORMER_H

#include "algeblah.h"
#include "reversemultitransformer.h"

namespace MathOps
{

template <typename T>
struct RearrangeMultiTransformer : public Visitor<T>
{
    RearrangeMultiTransformer(std::shared_ptr<MathOp<T>> solve_for, std::shared_ptr<MathOp<T>> from)
        : solve_for(solve_for), from(from)
    { }

    VisitorResult<T> visit(std::shared_ptr<ConstantSymbol<T>> op) override { return solve_for_single(op); }
    VisitorResult<T> visit(std::shared_ptr<Variable<T>> op) override { return solve_for_single(op); }
    VisitorResult<T> visit(std::shared_ptr<ValueVariable<T>> op) override { return solve_for_single(op); }
    VisitorResult<T> visit(std::shared_ptr<NamedConstant<T>> op) override { return solve_for_single(op); }
    VisitorResult<T> visit(std::shared_ptr<MutableValue<T>> op) override { return solve_for_single(op); }
    VisitorResult<T> visit(std::shared_ptr<ConstantValue<T>> op) override { return solve_for_single(op); }

    VisitorResult<T> visit(std::shared_ptr<Container<T>> op) override { return op->get_inner()->multi_transform(*this); }

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

    std::vector<std::shared_ptr<MathOp<T>>> solve_for_single(std::shared_ptr<MathOp<T>> op)
    {
        return op == solve_for
            ? std::vector<std::shared_ptr<MathOp<T>>> { from }
            : std::vector<std::shared_ptr<MathOp<T>>> { };
    }

    std::vector<std::shared_ptr<MathOp<T>>> solve_for_unary(std::shared_ptr<MathOp<T>> op, std::shared_ptr<MathOp<T>> x)
    {
        std::vector<std::shared_ptr<MathOp<T>>> solutions;
        auto from_x_results = op->multi_transform(ReverseMultiTransformer<T>(x, from));
        for(auto& from_x: from_x_results)
        {
            auto r = x->multi_transform(RearrangeMultiTransformer<T>(solve_for, from_x));
            solutions.insert(solutions.end(), r.begin(), r.end());
        }

        return solutions;
    }

    std::vector<std::shared_ptr<MathOp<T>>> solve_for_binary(std::shared_ptr<MathOp<T>> op,
        std::shared_ptr<MathOp<T>> lhs, std::shared_ptr<MathOp<T>> rhs)
    {
        std::vector<std::shared_ptr<MathOp<T>>> solutions;
        auto from_lhs_results = op->multi_transform(ReverseMultiTransformer<T>(lhs, from));
        for(auto& from_lhs: from_lhs_results)
        {
            auto r = lhs->multi_transform(RearrangeMultiTransformer<T>(solve_for, from_lhs));
            solutions.insert(solutions.end(), r.begin(), r.end());
        }

        auto from_rhs_results = op->multi_transform(ReverseMultiTransformer<T>(rhs, from));
        for(auto& from_rhs: from_rhs_results)
        {
            auto r = rhs->multi_transform(RearrangeMultiTransformer<T>(solve_for, from_rhs));
            solutions.insert(solutions.end(), r.begin(), r.end());
        }

        return solutions;
    }
};

} /* namespace MathOps */

#endif /* REARRANGEMULTITRANSFORMER_H */