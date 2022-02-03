#ifndef REVERSEMULTITRANSFORMER_H
#define REVERSEMULTITRANSFORMER_H

#include "algeblah.h"
#include "constants.h"

namespace MathOps
{

template <typename T>
struct ReverseMultiTransformer : public Visitor<T>
{
    ReverseMultiTransformer(std::shared_ptr<MathOp<T>> for_side, std::shared_ptr<MathOp<T>>from)
        : for_side(for_side), from(from)
    { }

    VisitorResult<T> visit(std::shared_ptr<ConstantSymbol<T>>) override { return std::vector<std::shared_ptr<MathOp<T>>> { }; }
    VisitorResult<T> visit(std::shared_ptr<Variable<T>>) override { return std::vector<std::shared_ptr<MathOp<T>>> { }; }
    VisitorResult<T> visit(std::shared_ptr<ValueVariable<T>>) override { return std::vector<std::shared_ptr<MathOp<T>>> { }; }
    VisitorResult<T> visit(std::shared_ptr<NamedConstant<T>>) override { return std::vector<std::shared_ptr<MathOp<T>>> { }; }
    VisitorResult<T> visit(std::shared_ptr<MutableValue<T>>) override { return std::vector<std::shared_ptr<MathOp<T>>> { }; }
    VisitorResult<T> visit(std::shared_ptr<ConstantValue<T>>) override { return std::vector<std::shared_ptr<MathOp<T>>> { }; }

    VisitorResult<T> visit(std::shared_ptr<Container<T>> op) override { return op->get_inner()->multi_transform(*this); }
 
    VisitorResult<T> visit(std::shared_ptr<Negate<T>> op)
    {
        return for_side == op->get_x()
            ? std::vector<std::shared_ptr<MathOp<T>>> { -from }
            : std::vector<std::shared_ptr<MathOp<T>>> { };
    }

    VisitorResult<T> visit(std::shared_ptr<Sqrt<T>> op)
    {
        return for_side == op->get_x()
            ? std::vector<std::shared_ptr<MathOp<T>>> { pow<T>(from, ConstantValue<T>::create(2.0)) }
            : std::vector<std::shared_ptr<MathOp<T>>> { };
    }

    VisitorResult<T> visit(std::shared_ptr<Log<T>> op)
    {
        return for_side == op->get_x()
            ? std::vector<std::shared_ptr<MathOp<T>>> { pow<T>(Constants::e<T>(), from) }
            : std::vector<std::shared_ptr<MathOp<T>>> { };
    }

    VisitorResult<T> visit(std::shared_ptr<Log10<T>> op)
    {
        return for_side == op->get_x()
            ? std::vector<std::shared_ptr<MathOp<T>>> { pow<T>(ConstantValue<T>::create(10.0), from) }
            : std::vector<std::shared_ptr<MathOp<T>>> { };
    }

    VisitorResult<T> visit(std::shared_ptr<Sin<T>> op)
    {
        return for_side == op->get_x()
            ? std::vector<std::shared_ptr<MathOp<T>>>{ asin(from) }
            : std::vector<std::shared_ptr<MathOp<T>>>{ };
    }

    VisitorResult<T> visit(std::shared_ptr<ASin<T>> op)
    {
        return for_side == op->get_x()
            ? std::vector<std::shared_ptr<MathOp<T>>>{ sin(from) }
            : std::vector<std::shared_ptr<MathOp<T>>>{};
    }

    VisitorResult<T> visit(std::shared_ptr<Cos<T>> op)
    {
        return for_side == op->get_x()
            ? std::vector<std::shared_ptr<MathOp<T>>>{ acos(from) }
            : std::vector<std::shared_ptr<MathOp<T>>>{ };
    }

    VisitorResult<T> visit(std::shared_ptr<ACos<T>> op)
    {
        return for_side == op->get_x()
            ? std::vector<std::shared_ptr<MathOp<T>>>{ cos(from) }
            : std::vector<std::shared_ptr<MathOp<T>>>{ };
    }

    VisitorResult<T> visit(std::shared_ptr<Tan<T>> op)
    {
        return for_side == op->get_x()
            ? std::vector<std::shared_ptr<MathOp<T>>>{ atan(from) }
            : std::vector<std::shared_ptr<MathOp<T>>>{ };
    }

    VisitorResult<T> visit(std::shared_ptr<ATan<T>> op)
    {
        return for_side == op->get_x()
            ? std::vector<std::shared_ptr<MathOp<T>>>{ tan(from) }
            : std::vector<std::shared_ptr<MathOp<T>>>{ };
    }

    VisitorResult<T> visit(std::shared_ptr<Sinh<T>> op)
    {
        return for_side == op->get_x()
            ? std::vector<std::shared_ptr<MathOp<T>>>{ asinh(from) }
            : std::vector<std::shared_ptr<MathOp<T>>>{ };
    }

    VisitorResult<T> visit(std::shared_ptr<ASinh<T>> op)
    {
        return for_side == op->get_x()
            ? std::vector<std::shared_ptr<MathOp<T>>>{ sinh(from) }
            : std::vector<std::shared_ptr<MathOp<T>>>{ };
    }

    VisitorResult<T> visit(std::shared_ptr<Cosh<T>> op)
    {
        return for_side == op->get_x()
            ? std::vector<std::shared_ptr<MathOp<T>>>{ acosh(from) }
            : std::vector<std::shared_ptr<MathOp<T>>>{ };
    }

    VisitorResult<T> visit(std::shared_ptr<ACosh<T>> op)
    {
        return for_side == op->get_x()
            ? std::vector<std::shared_ptr<MathOp<T>>>{ cosh(from) }
            : std::vector<std::shared_ptr<MathOp<T>>>{ };
    }

    VisitorResult<T> visit(std::shared_ptr<Tanh<T>> op)
    {
        return for_side == op->get_x()
            ? std::vector<std::shared_ptr<MathOp<T>>>{ atanh(from) }
            : std::vector<std::shared_ptr<MathOp<T>>>{ };
    }

    VisitorResult<T> visit(std::shared_ptr<ATanh<T>> op)
    {
        return for_side == op->get_x()
            ? std::vector<std::shared_ptr<MathOp<T>>>{ tanh(from) }
            : std::vector<std::shared_ptr<MathOp<T>>>{ };
    }

    VisitorResult<T> visit(std::shared_ptr<Pow<T>> op)
    {
        auto lhs = op->get_lhs();
        auto rhs = op->get_rhs();

        if (for_side == lhs)
        {
            if (rhs->is_constant())
            {
                if (rhs->result() == 2.0)
                {
                    return std::vector<std::shared_ptr<MathOp<T>>> { sqrt(from), -sqrt(from) };
                }

                T integral;
                if (MathOps::modf(rhs->result() / 2, integral) == 0)
                {
                    return std::vector<std::shared_ptr<MathOp<T>>> {
                         Pow<T>::create(from, (ConstantValue<T>::create(1.0) / rhs)),
                        -Pow<T>::create(from, (ConstantValue<T>::create(1.0) / rhs))};
                }
            }

            return std::vector<std::shared_ptr<MathOp<T>>> {
                 Pow<T>::create(from, (ConstantValue<T>::create(1.0) / rhs))
            };
        }
        else if (for_side == rhs)
        {
            return std::vector<std::shared_ptr<MathOp<T>>> { log(from) / log(lhs) };
        }
        else
        {
            return std::vector<std::shared_ptr<MathOp<T>>> { };
        }
    }

    VisitorResult<T> visit(std::shared_ptr<Mul<T>> op)
    {
        if      (for_side == op->get_lhs()) return std::vector<std::shared_ptr<MathOp<T>>> {from / op->get_rhs()};
        else if (for_side == op->get_rhs()) return std::vector<std::shared_ptr<MathOp<T>>> {from / op->get_lhs()};
        else                                return std::vector<std::shared_ptr<MathOp<T>>> { };
    }

    VisitorResult<T> visit(std::shared_ptr<Div<T>> op)
    {
        if      (for_side == op->get_lhs()) return std::vector<std::shared_ptr<MathOp<T>>> {from * op->get_rhs()};
        else if (for_side == op->get_rhs()) return std::vector<std::shared_ptr<MathOp<T>>> {op->get_lhs() / from};
        else                                return std::vector<std::shared_ptr<MathOp<T>>> { };
    }

    VisitorResult<T> visit(std::shared_ptr<Add<T>> op)
    {
        if      (for_side == op->get_lhs()) return std::vector<std::shared_ptr<MathOp<T>>> {from - op->get_rhs()};
        else if (for_side == op->get_rhs()) return std::vector<std::shared_ptr<MathOp<T>>> {from - op->get_lhs()};
        else                                return std::vector<std::shared_ptr<MathOp<T>>> { };
    }

    VisitorResult<T> visit(std::shared_ptr<Sub<T>> op)
    {
        if      (for_side == op->get_lhs()) return std::vector<std::shared_ptr<MathOp<T>>> {from + op->get_rhs()};
        else if (for_side == op->get_rhs()) return std::vector<std::shared_ptr<MathOp<T>>> {op->get_lhs() - from};
        else                                return std::vector<std::shared_ptr<MathOp<T>>> { };
    }

private:
    const std::shared_ptr<MathOp<T>> for_side;
    const std::shared_ptr<MathOp<T>> from;
};

} /* namespace MathOps */

#endif /* REVERSEMULTITRANSFORMER_H */