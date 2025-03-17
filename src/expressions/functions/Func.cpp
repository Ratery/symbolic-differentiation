#include "../expressions.hpp"

#include <format>

template<typename T>
Func<T>::Func(
    const std::shared_ptr<BaseExpr<T>>& _argument
) : argument(_argument) {}

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

template class FuncImpl<long double, SinFunc<long double>>;

template class FuncImpl<long double, CosFunc<long double>>;

template class FuncImpl<long double, LnFunc<long double>>;

template class FuncImpl<long double, ExpFunc<long double>>;
