#include "../expressions.hpp"

#include <format>

template<typename T>
BinOp<T>::BinOp(
    const std::shared_ptr<BaseExpr<T>>& _lhs,
    const std::shared_ptr<BaseExpr<T>>& _rhs
) : lhs(_lhs), rhs(_rhs) {}

template<typename T, typename Derived>
std::shared_ptr<BaseExpr<T>> BinOpImpl<T, Derived>::with_values(
    const std::unordered_map<std::string, T>& values
) const {
    return std::make_shared<Derived>(
        this->lhs->with_values(values),
        this->rhs->with_values(values)
    );
}

template<typename T, typename Derived>
std::string BinOpImpl<T, Derived>::to_string() const {
    std::string lhs_str = this->lhs->to_string();
    std::string rhs_str = this->rhs->to_string();

    std::shared_ptr<BinOp<T>> expr;
    if (((expr = std::dynamic_pointer_cast<BinOp<T>>(this->lhs))) &&
        (expr->precedence() < this->precedence())) {
        lhs_str = std::format("({})", lhs_str);
    }

    if (((expr = std::dynamic_pointer_cast<BinOp<T>>(this->rhs))) &&
        (expr->precedence() < this->precedence())) {
        rhs_str = std::format("({})", rhs_str);
    }

    return std::format("{} {} {}", lhs_str, this->name(), rhs_str);
}

template class BinOpImpl<long double, AddOp<long double>>;

template class BinOpImpl<long double, SubOp<long double>>;

template class BinOpImpl<long double, MulOp<long double>>;

template class BinOpImpl<long double, DivOp<long double>>;

template class BinOpImpl<long double, PowOp<long double>>;
