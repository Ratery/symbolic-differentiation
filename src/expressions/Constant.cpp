#include "expressions.hpp"

#include <format>
#include <stdexcept>

template<>
Constant<RealNumber>::Constant(const RealNumber _value) : value(_value) {}

template<>
Constant<ComplexNumber>::Constant(const ComplexNumber _value) : value(_value) {
    if (value.real() != 0 && value.imag() != 0) {
        throw std::invalid_argument(
            "When initializing constant with complex number its real or imaginary part must be 0."
        );
    }
}

template<typename T>
std::shared_ptr<BaseExpr<T>> Constant<T>::with_values(
    const std::unordered_map<std::string, T>& values
) const {
    return std::make_shared<Constant>(Constant(value));
}

template<>
std::string Constant<RealNumber>::to_string() const {
    return std::to_string(value);
}

template<>
std::string Constant<ComplexNumber>::to_string() const {
    if (value.real() != 0) {
        return std::to_string(value.real());
    }
    if (value.imag() != 0) {
        if (value.imag() == -1) {
            return "-i";
        }
        if (value.imag() == 1) {
            return "i";
        }
        return std::format("{}i", std::to_string(value.imag()));
    }
    return "0";  // we have an invariant that at least one of the parts is 0
}

template<typename T>
T Constant<T>::resolve() const {
    return value;
}

template<typename T>
std::shared_ptr<BaseExpr<T>> Constant<T>::diff(const std::string& by) const {
    return std::make_shared<Constant>(0);
}

template class Constant<RealNumber>;
template class Constant<ComplexNumber>;
