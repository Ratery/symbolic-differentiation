#include "expressions.hpp"

template<typename T>
Constant<T>::Constant(T _value) : value(_value) {}

template<typename T>
std::shared_ptr<BaseExpr<T>> Constant<T>::with_values(
    const std::unordered_map<std::string, T>& values
) const {
    return std::make_shared<Constant>(Constant(value));
}

template<typename T>
std::string Constant<T>::to_string() const {
    return std::to_string(value);
}

template<typename T>
T Constant<T>::resolve() const {
    return value;
}

template<typename T>
std::shared_ptr<BaseExpr<T>> Constant<T>::diff(const std::string& by) const {
    return std::make_shared<Constant>(0);
}

template class Constant<long double>;
