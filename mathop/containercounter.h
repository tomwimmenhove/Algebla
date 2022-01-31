#ifndef CONTAINERCOUNTER_H
#define CONTAINERCOUNTER_H

#include "counter.h"

#include <vector>

namespace MathOps
{

template <typename T>
struct ContainerCounter : public Counter<T, Container<T>>
{
    ContainerCounter(std::string name, int limit = 0)
        : Counter<T, Container<T>>(limit), name(name)
     { }

    static std::shared_ptr<Container<T>> find_first(std::shared_ptr<MathOp<T>> op, std::string name)
    {
        ContainerCounter<T> counter(name, 1);
        return op->count(counter) ? counter.get_results()[0] : nullptr;
    }

    VisitorResult<T> visit(std::shared_ptr<Container<T>> op) override
    {
        return count(op) + std::get<int>(Counter<T, Container<T>>::visit(op));
    }

private:
    const std::string name;

    int count(std::shared_ptr<Container<T>> op)
    {
        if (name.empty() || op->get_name() == name)
        {
            this->results.push_back(op);

            return 1;
        }

        return 0;
    }
};

} /* namespace MathOps */

#endif /* CONTAINERCOUNTER_H */