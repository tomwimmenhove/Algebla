#ifndef NAMEDVALUECOUNTER_H
#define NAMEDVALUECOUNTER_H

#include "counter.h"

#include <vector>

namespace MathOps
{

template <typename T>
struct NamedValueCounter : public Counter<T, Value<T>>
{
    NamedValueCounter(std::string symbol, int limit = 0)
        : Counter<T, Value<T>>(limit), symbol(symbol)
     { }

    static std::shared_ptr<Value<T>> find_first(std::shared_ptr<MathOp<T>> op, std::string symbol)
    {
        NamedValueCounter<T> counter(symbol, 1);
        return op->count(counter) ? counter.get_results()[0] : nullptr;
    }

    VisitorResult<T> visit(std::shared_ptr<Variable<T>> op) override { return count(op); }
    VisitorResult<T> visit(std::shared_ptr<NamedConstant<T>> op) override { return count(op); }
    VisitorResult<T> visit(std::shared_ptr<ValueVariable<T>> op) override { return count(op); }

private:
    std::string symbol;

    int count(std::shared_ptr<Value<T>> op)
    {
        if (symbol.empty() || op->get_name() == symbol)
        {
            this->results.push_back(op);

            return 1;
        }

        return 0;
    }
};

} /* namespace MathOps */

#endif /* NAMEDVALUECOUNTER_H */