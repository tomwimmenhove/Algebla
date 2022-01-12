#ifndef VARIABLECOUNTER_H
#define VARIABLECOUNTER_H

#include "dummycounter.h"

namespace MathOps
{

template <typename T>
struct VariableCounter : public DummyCounter<T>
{
    VariableCounter(std::string symbol)
        : symbol(symbol)
    { }

    VisitorResult<T> visit(std::shared_ptr<OpVariable<T>> op) override
    {
        return op->get_symbol() == symbol ? 1 : 0;
    }

private:
    std::string symbol;
};

} /* namespace MathOps */

#endif /* VARIABLECOUNTER_H */