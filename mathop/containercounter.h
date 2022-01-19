#ifndef CONTAINERCOUNTER_H
#define CONTAINERCOUNTER_H

#include "dummycounter.h"

#include <vector>

namespace MathOps
{

template <typename T>
struct ContainerCounter : public DummyCounter<T>
{
    ContainerCounter(std::string name, int limit = 0)
        : name(name), limit(limit)
     { }

    static std::shared_ptr<Container<T>> FindFirst(std::shared_ptr<MathOp<T>> op, std::string name)
    {
        ContainerCounter<T> counter(name, 1);
        if (op->count(counter) == 0)
        {
            return nullptr;
        }

        return counter.get_containers()[0];
    }

    VisitorResult<T> visit(std::shared_ptr<ConstantSymbol<T>> op) override
    {
        return !limit || (int) containers.size() < limit ? DummyCounter<T>::visit(op) : 0;
    }

    VisitorResult<T> visit(std::shared_ptr<MutableValue<T>> op) override
    {
        return !limit || (int) containers.size() < limit ? DummyCounter<T>::visit(op) : 0;
    }

    VisitorResult<T> visit(std::shared_ptr<ConstantValue<T>> op) override
    {
        return !limit || (int) containers.size() < limit ? DummyCounter<T>::visit(op) : 0;
    }

    VisitorResult<T> visit(std::shared_ptr<Container<T>> op) override
    {
        return !limit || (int) containers.size() < limit ? count(op) + std::get<int>(DummyCounter<T>::visit(op)) : 0;
    }

    VisitorResult<T> visit(std::shared_ptr<Negate<T>> op) override
    {
       return !limit || (int) containers.size() < limit ? DummyCounter<T>::visit(op) : 0;
    }

    VisitorResult<T> visit(std::shared_ptr<Sqrt<T>> op) override
    {
        return !limit || (int) containers.size() < limit ? DummyCounter<T>::visit(op) : 0;
    }

    VisitorResult<T> visit(std::shared_ptr<Log<T>> op) override
    {
        return !limit || (int) containers.size() < limit ? DummyCounter<T>::visit(op) : 0;
    }

    VisitorResult<T> visit(std::shared_ptr<Log10<T>> op) override
    {
        return !limit || (int) containers.size() < limit ? DummyCounter<T>::visit(op) : 0;
    }

    VisitorResult<T> visit(std::shared_ptr<Sin<T>> op) override
    {
        return !limit || (int) containers.size() < limit ? DummyCounter<T>::visit(op) : 0;
    }

    VisitorResult<T> visit(std::shared_ptr<ASin<T>> op) override
    {
        return !limit || (int) containers.size() < limit ? DummyCounter<T>::visit(op) : 0;
    }

    VisitorResult<T> visit(std::shared_ptr<Cos<T>> op) override
    {
        return !limit || (int) containers.size() < limit ? DummyCounter<T>::visit(op) : 0;
    }

    VisitorResult<T> visit(std::shared_ptr<ACos<T>> op) override
    {
        return !limit || (int) containers.size() < limit ? DummyCounter<T>::visit(op) : 0;
    }

    VisitorResult<T> visit(std::shared_ptr<Tan<T>> op) override
    {
        return !limit || (int) containers.size() < limit ? DummyCounter<T>::visit(op) : 0;
    }

    VisitorResult<T> visit(std::shared_ptr<ATan<T>> op) override
    {
        return !limit || (int) containers.size() < limit ? DummyCounter<T>::visit(op) : 0;
    }

    VisitorResult<T> visit(std::shared_ptr<Sinh<T>> op) override
    {
        return !limit || (int) containers.size() < limit ? DummyCounter<T>::visit(op) : 0;
    }

    VisitorResult<T> visit(std::shared_ptr<ASinh<T>> op) override
    {
        return !limit || (int) containers.size() < limit ? DummyCounter<T>::visit(op) : 0;
    }

    VisitorResult<T> visit(std::shared_ptr<Cosh<T>> op) override
    {
        return !limit || (int) containers.size() < limit ? DummyCounter<T>::visit(op) : 0;
    }

    VisitorResult<T> visit(std::shared_ptr<ACosh<T>> op) override
    {
        return !limit || (int) containers.size() < limit ? DummyCounter<T>::visit(op) : 0;
    }

    VisitorResult<T> visit(std::shared_ptr<Tanh<T>> op) override
    {
        return !limit || (int) containers.size() < limit ? DummyCounter<T>::visit(op) : 0;
    }

    VisitorResult<T> visit(std::shared_ptr<ATanh<T>> op) override
    {
        return !limit || (int) containers.size() < limit ? DummyCounter<T>::visit(op) : 0;
    }

    VisitorResult<T> visit(std::shared_ptr<Pow<T>> op) override
    {
        return !limit || (int) containers.size() < limit ? DummyCounter<T>::visit(op) : 0;
    }

    VisitorResult<T> visit(std::shared_ptr<Mul<T>> op) override
    {
        return !limit || (int) containers.size() < limit ? DummyCounter<T>::visit(op) : 0;
    }

    VisitorResult<T> visit(std::shared_ptr<Div<T>> op) override
    {
        return !limit || (int) containers.size() < limit ? DummyCounter<T>::visit(op) : 0;
    }

    VisitorResult<T> visit(std::shared_ptr<Add<T>> op) override
    {
        return !limit || (int) containers.size() < limit ? DummyCounter<T>::visit(op) : 0;
    }

    VisitorResult<T> visit(std::shared_ptr<Sub<T>> op) override
    {
        return !limit || (int) containers.size() < limit ? DummyCounter<T>::visit(op) : 0;
    }

    const std::vector<std::shared_ptr<Container<T>>>& get_containers() const { return containers; }

private:
    std::string name;
    int limit;
    std::vector<std::shared_ptr<Container<T>>> containers;

    int count(std::shared_ptr<Container<T>> op)
    {
        if (name.empty() || op->get_name() == name)
        {
            containers.push_back(op);

            return 1;
        }

        return 0;
    }
};

} /* namespace MathOps */

#endif /* CONTAINERCOUNTER_H */