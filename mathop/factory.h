#ifndef FACTORY_H
#define FACTORY_H

#include <string>
#include <memory>

namespace MathOps
{

/* Forward declarations */
template<typename T> struct MathOp;
template<typename T> struct External;
template<typename T> struct ConstantSymbol;
template<typename T> struct Variable;
template<typename T> struct ValueVariable;
template<typename T> struct NamedConstant;
template<typename T> struct MutableValue;
template<typename T> struct ConstantValue;

struct Factory
{
    template <typename T>
    static std::shared_ptr<MathOp<T>> CreateSymbolPi() { return ConstantSymbol<T>::create("%pi", get_constant_pi<T>()); }

    template <typename T>
    static std::shared_ptr<MathOp<T>> CreateSymbolE() { return ConstantSymbol<T>::create("%e", get_constant_e<T>()); }

    template <typename T>
    static std::shared_ptr<External<T>> CreateExternal(std::function<std::shared_ptr<MathOp<T>>()> fn, std::string name)
    {
        return External<T>::create(fn, name);
    }

    template <typename T>
    static std::shared_ptr<Variable<T>> CreateVariable(std::string symbol, T c = 0)
    {
        return Variable<T>::create(symbol, c);
    }

    template <typename T>
    static std::shared_ptr<ValueVariable<T>> CreateValueVariable(std::string symbol, T c = 0)
    {
        return ValueVariable<T>::create(symbol, c);
    }

    template <typename T>
    static std::shared_ptr<NamedConstant<T>> CreateNamedConstant(std::string symbol, T c = 0)
    {
        return NamedConstant<T>::create(symbol, c);
    }
    
    template <typename T>
    static std::shared_ptr<ConstantValue<T>> CreateConstantValue(T c)
    {
        return ConstantValue<T>::create(c);
    }

    template <typename T>
    static std::shared_ptr<MutableValue<T>> CreateMutableValue(T x)
    {
        return MutableValue<T>::create(x);
    }

private:
    Factory() = delete;
};

} /* MathOps */

#endif /* FACTORY_H */