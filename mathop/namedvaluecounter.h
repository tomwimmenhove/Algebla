#ifndef NAMEDVALUECOUNTER_H
#define NAMEDVALUECOUNTER_H

#include "dummycounter.h"

#include <vector>

namespace MathOps
{

template <typename T>
struct NamedValueCounter : public DummyCounter<T>
{
    NamedValueCounter(std::string symbol)
        : symbol(symbol)
    { }

    VisitorResult<T> visit(std::shared_ptr<Variable<T>> op) override { return count(op); }
    VisitorResult<T> visit(std::shared_ptr<NamedConstant<T>> op) override { return count(op); }
    VisitorResult<T> visit(std::shared_ptr<ValueVariable<T>> op) override { return count(op); }

    const std::vector<std::shared_ptr<Value<T>>>& get_variables() const { return variables; }

private:
    std::string symbol;
    std::vector<std::shared_ptr<Value<T>>> variables;

    int count(std::shared_ptr<Value<T>> op)
    {
        if (op->get_symbol() != symbol)
        {
            return 0;
        }

        variables.push_back(op);

        return 1;
    }
};

} /* namespace MathOps */

#endif /* NAMEDVALUECOUNTER_H */