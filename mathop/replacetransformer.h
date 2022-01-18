#ifndef REPLACETRANSFORMER_H
#define REPLACETRANSFORMER_H

#include "dummytransformer.h"

namespace MathOps
{

template <typename T>
struct ReplaceTransformer : DummyTransformer<T>
{
    ReplaceTransformer(std::shared_ptr<MathOp<T>> subject, std::shared_ptr<MathOp<T>> replacement)
        : subject(subject), replacement(replacement)
    { }

    VisitorResult<T> visit(std::shared_ptr<ConstantSymbol<T>> op) override
    {
        return op == subject ? replacement : op;
    }

    VisitorResult<T> visit(std::shared_ptr<Variable<T>> op) override
    {
        return op == subject ? replacement : op;
    }

    VisitorResult<T> visit(std::shared_ptr<ValueVariable<T>> op) override
    {
        return op == subject ? replacement : op;
    }

    VisitorResult<T> visit(std::shared_ptr<NamedConstant<T>> op) override
    {
        return op == subject ? replacement : op;
    }

    VisitorResult<T> visit(std::shared_ptr<MutableValue<T>> op) override
    {
        return op == subject ? replacement : op;
    }

    VisitorResult<T> visit(std::shared_ptr<ConstantValue<T>> op) override
    {
        return op == subject ? replacement : op;
    }

    VisitorResult<T> visit(std::shared_ptr<External<T>> op) override
    {
        if (op == subject)
        {
            return replacement;
        }

        return DummyTransformer<T>::visit(op);
    }

    VisitorResult<T> visit(std::shared_ptr<Negate<T>> op) override
    {
        if (op == subject)
        {
            return replacement;
        }

        return DummyTransformer<T>::visit(op);
    }

    VisitorResult<T> visit(std::shared_ptr<Sqrt<T>> op) override
    {
        if (op == subject)
        {
            return replacement;
        }

        return DummyTransformer<T>::visit(op);
    }

    VisitorResult<T> visit(std::shared_ptr<Log<T>> op) override
    {
        if (op == subject)
        {
            return replacement;
        }
        
        return DummyTransformer<T>::visit(op);
    }

    VisitorResult<T> visit(std::shared_ptr<Log10<T>> op) override
    {
        if (op == subject)
        {
            return replacement;
        }
        
        return DummyTransformer<T>::visit(op);
    }

    VisitorResult<T> visit(std::shared_ptr<Sin<T>> op) override
    {
        if (op == subject)
        {
            return replacement;
        }
        
        return DummyTransformer<T>::visit(op);
    }

    VisitorResult<T> visit(std::shared_ptr<ASin<T>> op) override
    {
        if (op == subject)
        {
            return replacement;
        }
        
        return DummyTransformer<T>::visit(op);
    }

    VisitorResult<T> visit(std::shared_ptr<Cos<T>> op) override
    {
        if (op == subject)
        {
            return replacement;
        }
        
        return DummyTransformer<T>::visit(op);
    }

    VisitorResult<T> visit(std::shared_ptr<ACos<T>> op) override
    {
        if (op == subject)
        {
            return replacement;
        }
        
        return DummyTransformer<T>::visit(op);
    }

    VisitorResult<T> visit(std::shared_ptr<Tan<T>> op) override
    {
        if (op == subject)
        {
            return replacement;
        }
        
        return DummyTransformer<T>::visit(op);
    }

    VisitorResult<T> visit(std::shared_ptr<ATan<T>> op) override
    {
        if (op == subject)
        {
            return replacement;
        }
        
        return DummyTransformer<T>::visit(op);
    }

    VisitorResult<T> visit(std::shared_ptr<Sinh<T>> op) override
    {
        if (op == subject)
        {
            return replacement;
        }
        
        return DummyTransformer<T>::visit(op);
    }

    VisitorResult<T> visit(std::shared_ptr<ASinh<T>> op) override
    {
        if (op == subject)
        {
            return replacement;
        }
        
        return DummyTransformer<T>::visit(op);
    }

    VisitorResult<T> visit(std::shared_ptr<Cosh<T>> op) override
    {
        if (op == subject)
        {
            return replacement;
        }
        
        return DummyTransformer<T>::visit(op);
    }

    VisitorResult<T> visit(std::shared_ptr<ACosh<T>> op) override
    {
        if (op == subject)
        {
            return replacement;
        }
        
        return DummyTransformer<T>::visit(op);
    }

    VisitorResult<T> visit(std::shared_ptr<Tanh<T>> op) override
    {
        if (op == subject)
        {
            return replacement;
        }
        
        return DummyTransformer<T>::visit(op);
    }

    VisitorResult<T> visit(std::shared_ptr<ATanh<T>> op) override
    {
        if (op == subject)
        {
            return replacement;
        }
        
        return DummyTransformer<T>::visit(op);
    }

    VisitorResult<T> visit(std::shared_ptr<Pow<T>> op) override
    {
        if (op == subject)
        {
            return replacement;
        }
        
        return DummyTransformer<T>::visit(op);
    }

    VisitorResult<T> visit(std::shared_ptr<Mul<T>> op) override
    {
        if (op == subject)
        {
            return replacement;
        }
        
        return DummyTransformer<T>::visit(op);
    }

    VisitorResult<T> visit(std::shared_ptr<Div<T>> op) override
    {
        if (op == subject)
        {
            return replacement;
        }
        
        return DummyTransformer<T>::visit(op);
    }

    VisitorResult<T> visit(std::shared_ptr<Add<T>> op) override
    {
        if (op == subject)
        {
            return replacement;
        }
        
        return DummyTransformer<T>::visit(op);
    }

    VisitorResult<T> visit(std::shared_ptr<Sub<T>> op) override
    {
        if (op == subject)
        {
            return replacement;
        }
        
        return DummyTransformer<T>::visit(op);
    }

private:
    std::shared_ptr<MathOp<T>> subject;
    std::shared_ptr<MathOp<T>> replacement;
};

} /* namespace MathOps */

#endif /* REPLACETRANSFORMER_H */