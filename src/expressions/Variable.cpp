#include "expressions.hpp"

#include <utility>
#include <format>

template<typename T>
Variable<T>::Variable(std::string _name) : name(std::move(_name)) {}

template<typename T>
std::shared_ptr<BaseExpr<T>> Variable<T>::with_values(
    const std::unordered_map<std::string, T>& values
) const {
    if (values.contains(name)) {
        return std::make_shared<Constant<T>>(values.at(name));
    }
    return std::make_shared<Variable>(*this);
}

template<typename T>
std::string Variable<T>::to_string() const {
    return name;
}

template<typename T>
T Variable<T>::resolve() const {
    throw std::runtime_error(std::format("Can not resolve variable \"{}\"", name));
}

template<typename T>
std::shared_ptr<BaseExpr<T>> Variable<T>::diff(const std::string& by) const {
    return std::make_shared<Constant<T>>(by == name ? 1 : 0);
}

template class Variable<long double>;
