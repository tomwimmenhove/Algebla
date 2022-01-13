#ifndef VARIABLECOUNTER_H
#define VARIABLECOUNTER_H

#include "dummycounter.h"

#include <vector>

namespace MathOps
{

template <typename T>
struct VariableCounter : public DummyCounter<T>
{
    VariableCounter(std::string symbol)
        : symbol(symbol)
    { }

    VisitorResult<T> visit(std::shared_ptr<Variable<T>> op) override
    {
        if (op->get_symbol() != symbol)
        {
            return 0;
        }

        variables.push_back(op);

        return 1;
    }

    const std::vector<std::shared_ptr<Variable<T>>>& get_variables() const { return variables; }

private:
    std::string symbol;
    std::vector<std::shared_ptr<Variable<T>>> variables;
};

} /* namespace MathOps */

#endif /* VARIABLECOUNTER_H */