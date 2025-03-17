#include "../expressions.hpp"

#include <format>

template<typename T>
Func<T>::Func(
    const std::shared_ptr<BaseExpr<T>>& _argument
) : argument(_argument) {}

template<typename T>
std::shared_ptr<Func<T>> Func<T>::from_name(
    const std::string& name,
    const std::shared_ptr<BaseExpr<T>>& _argument
) {
    if (name == "sin") {
        return std::make_shared<SinFunc<T>>(_argument);
    }
    if (name == "cos") {
        return std::make_shared<CosFunc<T>>(_argument);
    }
    if (name == "ln") {
        return std::make_shared<LnFunc<T>>(_argument);
    }
    if (name == "exp") {
        return std::make_shared<ExpFunc<T>>(_argument);
    }
    throw std::runtime_error(std::format("Unknown function: \"{}\"", name));
}

template<typename T, typename Derived>
std::shared_ptr<BaseExpr<T>> FuncImpl<T, Derived>::with_values(
    const std::unordered_map<std::string, T>& values
) const {
    return std::make_shared<Derived>(this->argument->with_values(values));
}

template<typename T, typename Derived>
std::string FuncImpl<T, Derived>::to_string() const {
    return std::format("{}({})", this->name(), this->argument->to_string());
}

template class Func<RealNumber>;
template class Func<ComplexNumber>;

template class FuncImpl<RealNumber, SinFunc<RealNumber>>;
template class FuncImpl<ComplexNumber, SinFunc<ComplexNumber>>;

template class FuncImpl<RealNumber, CosFunc<RealNumber>>;
template class FuncImpl<ComplexNumber, CosFunc<ComplexNumber>>;

template class FuncImpl<RealNumber, LnFunc<RealNumber>>;
template class FuncImpl<ComplexNumber, LnFunc<ComplexNumber>>;

template class FuncImpl<RealNumber, ExpFunc<RealNumber>>;
template class FuncImpl<ComplexNumber, ExpFunc<ComplexNumber>>;
