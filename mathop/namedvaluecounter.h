#ifndef NAMEDVALUECOUNTER_H
#define NAMEDVALUECOUNTER_H

#include "dummycounter.h"

#include <vector>

namespace MathOps
{

template <typename T>
struct NamedValueCounter : public DummyCounter<T>
{
    NamedValueCounter(std::string symbol, int limit = 0)
        : symbol(symbol), limit(limit)
     { }

    static std::shared_ptr<Value<T>> FindFirst(std::shared_ptr<MathOp<T>> op, std::string symbol)
    {
        NamedValueCounter<T> counter(symbol, 1);
        if (op->count(counter) == 0)
        {
            return nullptr;
        }

        return counter.get_variables()[0];
    }

    VisitorResult<T> visit(std::shared_ptr<Variable<T>> op) override { return count(op); }
    VisitorResult<T> visit(std::shared_ptr<NamedConstant<T>> op) override { return count(op); }
    VisitorResult<T> visit(std::shared_ptr<ValueVariable<T>> op) override { return count(op); }

    VisitorResult<T> visit(std::shared_ptr<ConstantSymbol<T>> op) override
    {
        return !limit || (int) variables.size() < limit ? DummyCounter<T>::visit(op) : 0;
    }

    VisitorResult<T> visit(std::shared_ptr<MutableValue<T>> op) override
    {
        return !limit || (int) variables.size() < limit ? DummyCounter<T>::visit(op) : 0;
    }

    VisitorResult<T> visit(std::shared_ptr<ConstantValue<T>> op) override
    {
        return !limit || (int) variables.size() < limit ? DummyCounter<T>::visit(op) : 0;
    }

    VisitorResult<T> visit(std::shared_ptr<Container<T>> op) override
    {
        return !limit || (int) variables.size() < limit ? DummyCounter<T>::visit(op) : 0;
    }

    VisitorResult<T> visit(std::shared_ptr<Negate<T>> op) override
    {
       return !limit || (int) variables.size() < limit ? DummyCounter<T>::visit(op) : 0;
    }

    VisitorResult<T> visit(std::shared_ptr<Sqrt<T>> op) override
    {
        return !limit || (int) variables.size() < limit ? DummyCounter<T>::visit(op) : 0;
    }

    VisitorResult<T> visit(std::shared_ptr<Log<T>> op) override
    {
        return !limit || (int) variables.size() < limit ? DummyCounter<T>::visit(op) : 0;
    }

    VisitorResult<T> visit(std::shared_ptr<Log10<T>> op) override
    {
        return !limit || (int) variables.size() < limit ? DummyCounter<T>::visit(op) : 0;
    }

    VisitorResult<T> visit(std::shared_ptr<Sin<T>> op) override
    {
        return !limit || (int) variables.size() < limit ? DummyCounter<T>::visit(op) : 0;
    }

    VisitorResult<T> visit(std::shared_ptr<ASin<T>> op) override
    {
        return !limit || (int) variables.size() < limit ? DummyCounter<T>::visit(op) : 0;
    }

    VisitorResult<T> visit(std::shared_ptr<Cos<T>> op) override
    {
        return !limit || (int) variables.size() < limit ? DummyCounter<T>::visit(op) : 0;
    }

    VisitorResult<T> visit(std::shared_ptr<ACos<T>> op) override
    {
        return !limit || (int) variables.size() < limit ? DummyCounter<T>::visit(op) : 0;
    }

    VisitorResult<T> visit(std::shared_ptr<Tan<T>> op) override
    {
        return !limit || (int) variables.size() < limit ? DummyCounter<T>::visit(op) : 0;
    }

    VisitorResult<T> visit(std::shared_ptr<ATan<T>> op) override
    {
        return !limit || (int) variables.size() < limit ? DummyCounter<T>::visit(op) : 0;
    }

    VisitorResult<T> visit(std::shared_ptr<Sinh<T>> op) override
    {
        return !limit || (int) variables.size() < limit ? DummyCounter<T>::visit(op) : 0;
    }

    VisitorResult<T> visit(std::shared_ptr<ASinh<T>> op) override
    {
        return !limit || (int) variables.size() < limit ? DummyCounter<T>::visit(op) : 0;
    }

    VisitorResult<T> visit(std::shared_ptr<Cosh<T>> op) override
    {
        return !limit || (int) variables.size() < limit ? DummyCounter<T>::visit(op) : 0;
    }

    VisitorResult<T> visit(std::shared_ptr<ACosh<T>> op) override
    {
        return !limit || (int) variables.size() < limit ? DummyCounter<T>::visit(op) : 0;
    }

    VisitorResult<T> visit(std::shared_ptr<Tanh<T>> op) override
    {
        return !limit || (int) variables.size() < limit ? DummyCounter<T>::visit(op) : 0;
    }

    VisitorResult<T> visit(std::shared_ptr<ATanh<T>> op) override
    {
        return !limit || (int) variables.size() < limit ? DummyCounter<T>::visit(op) : 0;
    }

    VisitorResult<T> visit(std::shared_ptr<Pow<T>> op) override
    {
        return !limit || (int) variables.size() < limit ? DummyCounter<T>::visit(op) : 0;
    }

    VisitorResult<T> visit(std::shared_ptr<Mul<T>> op) override
    {
        return !limit || (int) variables.size() < limit ? DummyCounter<T>::visit(op) : 0;
    }

    VisitorResult<T> visit(std::shared_ptr<Div<T>> op) override
    {
        return !limit || (int) variables.size() < limit ? DummyCounter<T>::visit(op) : 0;
    }

    VisitorResult<T> visit(std::shared_ptr<Add<T>> op) override
    {
        return !limit || (int) variables.size() < limit ? DummyCounter<T>::visit(op) : 0;
    }

    VisitorResult<T> visit(std::shared_ptr<Sub<T>> op) override
    {
        return !limit || (int) variables.size() < limit ? DummyCounter<T>::visit(op) : 0;
    }

    const std::vector<std::shared_ptr<Value<T>>>& get_variables() const { return variables; }

private:
    std::string symbol;
    int limit;
    std::vector<std::shared_ptr<Value<T>>> variables;

    int count(std::shared_ptr<Value<T>> op)
    {
        if (symbol.empty() || op->get_symbol() == symbol)
        {
            variables.push_back(op);

            return 1;
        }

        return 0;
    }
};

} /* namespace MathOps */

#endif /* NAMEDVALUECOUNTER_H */