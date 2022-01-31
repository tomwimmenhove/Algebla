#ifndef COUNTER_H
#define COUNTER_H

#include "algeblah.h"

namespace MathOps
{

template <typename T, typename U>
struct Counter : public Visitor<T>
{
    virtual VisitorResult<T> visit(std::shared_ptr<Variable<T>>) override { return 0; }
    virtual VisitorResult<T> visit(std::shared_ptr<ConstantSymbol<T>>) override { return 0; }
    virtual VisitorResult<T> visit(std::shared_ptr<ValueVariable<T>>) override { return 0; }
    virtual VisitorResult<T> visit(std::shared_ptr<NamedConstant<T>>) override { return 0; }
    virtual VisitorResult<T> visit(std::shared_ptr<MutableValue<T>>) override { return 0; }
    virtual VisitorResult<T> visit(std::shared_ptr<ConstantValue<T>>) override { return 0; }

    virtual VisitorResult<T> visit(std::shared_ptr<Container<T>> op) override
    {
        return !limit || (int) results.size() < limit ? op->get_inner()->count(*this) : 0;
    }

    virtual VisitorResult<T> visit(std::shared_ptr<Negate<T>> op) override { return count(op); }
    virtual VisitorResult<T> visit(std::shared_ptr<Sqrt<T>> op) override { return count(op); }
    virtual VisitorResult<T> visit(std::shared_ptr<Log<T>> op) override { return count(op); }
    virtual VisitorResult<T> visit(std::shared_ptr<Log10<T>> op) override { return count(op); }
    virtual VisitorResult<T> visit(std::shared_ptr<Sin<T>> op) override { return count(op); }
    virtual VisitorResult<T> visit(std::shared_ptr<ASin<T>> op) override { return count(op); }
    virtual VisitorResult<T> visit(std::shared_ptr<Cos<T>> op) override { return count(op); }
    virtual VisitorResult<T> visit(std::shared_ptr<ACos<T>> op) override { return count(op); }
    virtual VisitorResult<T> visit(std::shared_ptr<Tan<T>> op) override { return count(op); }
    virtual VisitorResult<T> visit(std::shared_ptr<ATan<T>> op) override { return count(op); }
    virtual VisitorResult<T> visit(std::shared_ptr<Sinh<T>> op) override { return count(op); }
    virtual VisitorResult<T> visit(std::shared_ptr<ASinh<T>> op) override { return count(op); }
    virtual VisitorResult<T> visit(std::shared_ptr<Cosh<T>> op) override { return count(op); }
    virtual VisitorResult<T> visit(std::shared_ptr<ACosh<T>> op) override { return count(op); }
    virtual VisitorResult<T> visit(std::shared_ptr<Tanh<T>> op) override { return count(op); }
    virtual VisitorResult<T> visit(std::shared_ptr<ATanh<T>> op) override { return count(op); }
    virtual VisitorResult<T> visit(std::shared_ptr<Pow<T>> op) override { return count(op); }
    virtual VisitorResult<T> visit(std::shared_ptr<Mul<T>> op) override { return count(op); }
    virtual VisitorResult<T> visit(std::shared_ptr<Div<T>> op) override { return count(op); }
    virtual VisitorResult<T> visit(std::shared_ptr<Add<T>> op) override { return count(op); }
    virtual VisitorResult<T> visit(std::shared_ptr<Sub<T>> op) override { return count(op); }

    const std::vector<std::shared_ptr<U>>& get_results() const { return results; }

protected:
     Counter(int limit = 0)
        : limit(limit)
     { }

    const int limit;
    std::vector<std::shared_ptr<U>> results;

private:
    VisitorResult<T> count(std::shared_ptr<MathUnaryOp<T>> op)
    {
        return !limit || (int) results.size() < limit ? op->get_x()->count(*this) : 0;        
    }

    VisitorResult<T> count(std::shared_ptr<MathBinaryOp<T>> op)
    {
        return !limit || (int) results.size() < limit ? op->get_lhs()->count(*this) + op->get_rhs()->count(*this) : 0;
    }
};

} /* namespace MathOps */

#endif /* COUNTER_H */