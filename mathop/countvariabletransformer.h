#ifndef COUNTVARIABLETRANSFORMER_H
#define COUNTVARIABLETRANSFORMER_H

#include "dummytransformer.h"

namespace MathOps
{

template <typename T>
struct CountVariableTransformer : public DummyTransformer<T>
{
    CountVariableTransformer(std::string symbol)
        : n(0), symbol(symbol)
    { }

    int get_count() const { return n; }

    virtual std::shared_ptr<MathOp<T>> visit(std::shared_ptr<OpVariable<T>> op) override
    {
        if (op->get_symbol() == symbol)
        {
            n++;
        }

        return op;
    }

private:
    int n;
    std::string symbol;
};

} /* namespace MathOps */

#endif /* COUNTVARIABLETRANSFORMER_H */