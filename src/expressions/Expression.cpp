#include "expressions.hpp"
#include "../parser/Parser.hpp"

#include <utility>

template<typename T>
Expression<T>::Expression(std::shared_ptr<BaseExpr<T>> expression_impl)
    : inner(std::move(expression_impl)) {}

template<>
Expression<RealNumber>::Expression(RealNumber number)
    : inner(std::make_shared<Constant<RealNumber>>(number)) {}

template<>
Expression<ComplexNumber>::Expression(ComplexNumber number) {
    if (number.real() == 0 || number.imag() == 0) {
        inner = std::make_shared<Constant<ComplexNumber>>(number);
    } else {
        inner = std::make_shared<AddOp<ComplexNumber>>(
            std::make_shared<Constant<ComplexNumber>>(ComplexNumber(number.real(), 0)),
            std::make_shared<Constant<ComplexNumber>>(ComplexNumber(0, number.imag()))
        );
    }
}

template<typename T>
Expression<T> Expression<T>::from_string(const std::string& expression_str, bool case_sensitive) {
    return Parser<T>(expression_str, case_sensitive).parse();
}

template<typename T>
Expression<T>::Expression(const std::string& var_name)
    : inner(std::make_shared<Variable<T>>(var_name)) {}

template<typename T>
Expression<T>& Expression<T>::operator=(const Expression& rhs) {
    if (this != &rhs) {
        inner = rhs.inner;
    }
    return *this;
}

template<typename T>
Expression<T> Expression<T>::sin() const {
    return Expression(std::make_shared<SinFunc<T>>(inner));
}

template<typename T>
Expression<T> Expression<T>::cos() const {
    return Expression(std::make_shared<CosFunc<T>>(inner));
}

template<typename T>
Expression<T> Expression<T>::ln() const {
    return Expression(std::make_shared<LnFunc<T>>(inner));
}

template<typename T>
Expression<T> Expression<T>::exp() const {
    return Expression(std::make_shared<ExpFunc<T>>(inner));
}

template<typename T>
Expression<T> Expression<T>::operator+(const Expression& rhs) const {
    return Expression(std::make_shared<AddOp<T>>(inner, rhs.inner));
}

template<typename T>
Expression<T>& Expression<T>::operator+=(const Expression& rhs) {
    *this = *this + rhs;
    return *this;
}

template<typename T>
Expression<T> Expression<T>::operator-(const Expression& rhs) const {
    return Expression(std::make_shared<SubOp<T>>(inner, rhs.inner));
}

template<typename T>
Expression<T>& Expression<T>::operator-=(const Expression& rhs) {
    *this = *this - rhs;
    return *this;
}

template<typename T>
Expression<T> Expression<T>::operator*(const Expression& rhs) const {
    return Expression(std::make_shared<MulOp<T>>(inner, rhs.inner));
}

template<typename T>
Expression<T>& Expression<T>::operator*=(const Expression& rhs) {
    *this = *this * rhs;
    return *this;
}

template<typename T>
Expression<T> Expression<T>::operator/(const Expression& rhs) const {
    return Expression(std::make_shared<DivOp<T>>(inner, rhs.inner));
}

template<typename T>
Expression<T>& Expression<T>::operator/=(const Expression& rhs) {
    *this = *this / rhs;
    return *this;
}

template<typename T>
Expression<T> Expression<T>::operator^(const Expression& rhs) const {
    return Expression(std::make_shared<PowOp<T>>(inner, rhs.inner));
}

template<typename T>
Expression<T>& Expression<T>::operator^=(const Expression& rhs) {
    *this = *this ^ rhs;
    return *this;
}

template<typename T>
Expression<T> Expression<T>::with_values(std::unordered_map<std::string, T> &values) const {
    return Expression(inner->with_values(values));
}

template<typename T>
T Expression<T>::resolve() const {
    return inner->resolve();
}

template<typename T>
T Expression<T>::resolve_with(std::unordered_map<std::string, T> &values) const {
    return this->with_values(values).resolve();
}

template<typename T>
Expression<T> Expression<T>::diff(const std::string& by) const {
    return Expression(inner->diff(by));
}

template<typename T>
std::string Expression<T>::to_string() const {
    return inner->to_string();
}

template class Expression<RealNumber>;
template class Expression<ComplexNumber>;
