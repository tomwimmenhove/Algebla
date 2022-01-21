#ifndef EXPANDTRANSFORMER_H
#define EXPANDTRANSFORMER_H

#include "dummytransformer.h"

namespace MathOps
{

template <typename T>
struct ExpandTransformer : public DummyTransformer<T>
{
    virtual VisitorResult<T> visit(std::shared_ptr<Container<T>> op) override
    {
        return op->get_inner()->transform(*this);
    }
};

} /* namespace MathOps */

#endif /* EXPANDTRANSFORMER_H */