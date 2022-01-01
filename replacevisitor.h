#ifndef REPLACEVISITOR_H
#define REPLACEVISITOR_H

#include "dummyvisitor.h"

template <typename T>
struct MathOpReplaceVisitor : public MathOpDummyVisitor<T>
{
    MathOpReplaceVisitor(std::shared_ptr<MathOp<T>> subject, std::shared_ptr<MathOp<T>> replacement)
        : subject(subject), replacement(replacement)
    { }

    virtual std::shared_ptr<MathOp<T>> visit(std::shared_ptr<MathOpMutableSymbol<T>> op) override
    {
        return op == subject ? replacement : op;
    }

    virtual std::shared_ptr<MathOp<T>> visit(std::shared_ptr<MathOpConstantSymbol<T>> op) override
    {
        return op == subject ? replacement : op;
    }

    virtual std::shared_ptr<MathOp<T>> visit(std::shared_ptr<MathOpVariable<T>> op) override
    {
        return op == subject ? replacement : op;
    }

    virtual std::shared_ptr<MathOp<T>> visit(std::shared_ptr<MathOpMutableValue<T>> op) override
    {
        return op == subject ? replacement : op;
    }

    virtual std::shared_ptr<MathOp<T>> visit(std::shared_ptr<MathOpConstantValue<T>> op) override
    {
        return op == subject ? replacement : op;
    }

    virtual std::shared_ptr<MathOp<T>> visit(std::shared_ptr<MathOpSqrt<T>> op) override
    {
        if (op == subject)
        {
            return replacement;
        }

        return MathOpDummyVisitor<T>::visit(op);
    }

    virtual std::shared_ptr<MathOp<T>> visit(std::shared_ptr<MathOpSquare<T>> op) override
    {
        if (op == subject)
        {
            return replacement;
        }
        
        return MathOpDummyVisitor<T>::visit(op);
    }

    virtual std::shared_ptr<MathOp<T>> visit(std::shared_ptr<MathOpLog<T>> op) override
    {
        if (op == subject)
        {
            return replacement;
        }
        
        return MathOpDummyVisitor<T>::visit(op);
    }

    virtual std::shared_ptr<MathOp<T>> visit(std::shared_ptr<MathOpSin<T>> op) override
    {
        if (op == subject)
        {
            return replacement;
        }
        
        return MathOpDummyVisitor<T>::visit(op);
    }

    virtual std::shared_ptr<MathOp<T>> visit(std::shared_ptr<MathOpASin<T>> op) override
    {
        if (op == subject)
        {
            return replacement;
        }
        
        return MathOpDummyVisitor<T>::visit(op);
    }

    virtual std::shared_ptr<MathOp<T>> visit(std::shared_ptr<MathOpCos<T>> op) override
    {
        if (op == subject)
        {
            return replacement;
        }
        
        return MathOpDummyVisitor<T>::visit(op);
    }

    virtual std::shared_ptr<MathOp<T>> visit(std::shared_ptr<MathOpACos<T>> op) override
    {
        if (op == subject)
        {
            return replacement;
        }
        
        return MathOpDummyVisitor<T>::visit(op);
    }

    virtual std::shared_ptr<MathOp<T>> visit(std::shared_ptr<MathOpTan<T>> op) override
    {
        if (op == subject)
        {
            return replacement;
        }
        
        return MathOpDummyVisitor<T>::visit(op);
    }

    virtual std::shared_ptr<MathOp<T>> visit(std::shared_ptr<MathOpATan<T>> op) override
    {
        if (op == subject)
        {
            return replacement;
        }
        
        return MathOpDummyVisitor<T>::visit(op);
    }

    virtual std::shared_ptr<MathOp<T>> visit(std::shared_ptr<MathOpPow<T>> op) override
    {
        if (op == subject)
        {
            return replacement;
        }
        
        return MathOpDummyVisitor<T>::visit(op);
    }

    virtual std::shared_ptr<MathOp<T>> visit(std::shared_ptr<MathOpMul<T>> op) override
    {
        if (op == subject)
        {
            return replacement;
        }
        
        return MathOpDummyVisitor<T>::visit(op);
    }

    virtual std::shared_ptr<MathOp<T>> visit(std::shared_ptr<MathOpDiv<T>> op) override
    {
        if (op == subject)
        {
            return replacement;
        }
        
        return MathOpDummyVisitor<T>::visit(op);
    }

    virtual std::shared_ptr<MathOp<T>> visit(std::shared_ptr<MathOpAdd<T>> op) override
    {
        if (op == subject)
        {
            return replacement;
        }
        
        return MathOpDummyVisitor<T>::visit(op);
    }

    virtual std::shared_ptr<MathOp<T>> visit(std::shared_ptr<MathOpSub<T>> op) override
    {
        if (op == subject)
        {
            return replacement;
        }
        
        return MathOpDummyVisitor<T>::visit(op);
    }

private:
    std::shared_ptr<MathOp<T>> subject;
    std::shared_ptr<MathOp<T>> replacement;
};

#endif /* REPLACEVISITOR_H */