#include "../expressions.hpp"

#include <format>

template<typename T>
BinOp<T>::BinOp(
    const std::shared_ptr<BaseExpr<T>>& _lhs,
    const std::shared_ptr<BaseExpr<T>>& _rhs
) : lhs(_lhs), rhs(_rhs) {}

template<typename T>
OpPrecedence BinOp<T>::get_precedence_by_name(const std::string& name) {
    if (name == "+" || name == "-") {
        return OpPrecedence::AddSub;
    }
    if (name == "*") {
        return OpPrecedence::Mul;
    }
    if (name == "/") {
        return OpPrecedence::Div;
    }
    if (name == "^") {
        return OpPrecedence::Pow;
    }
    throw std::runtime_error(std::format("Unknown binary operator: \"{}\"", name));
}

template<typename T>
std::shared_ptr<BinOp<T>> BinOp<T>::from_name(
    const std::string& name,
    const std::shared_ptr<BaseExpr<T>>& _lhs,
    const std::shared_ptr<BaseExpr<T>>& _rhs
) {
    if (name == "+") {
        return std::make_shared<AddOp<T>>(_lhs, _rhs);
    }
    if (name == "-") {
        return std::make_shared<SubOp<T>>(_lhs, _rhs);
    }
    if (name == "*") {
        return std::make_shared<MulOp<T>>(_lhs, _rhs);
    }
    if (name == "/") {
        return std::make_shared<DivOp<T>>(_lhs, _rhs);
    }
    if (name == "^") {
        return std::make_shared<PowOp<T>>(_lhs, _rhs);
    }
    throw std::runtime_error(std::format("Unknown binary operator: \"{}\"", name));
}

template<typename T, typename Derived>
OpPrecedence BinOpImpl<T, Derived>::precedence() const {
    return BinOp<T>::get_precedence_by_name(this->name());
}

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

template class BinOp<RealNumber>;
template class BinOp<ComplexNumber>;

template class BinOpImpl<RealNumber, AddOp<RealNumber>>;
template class BinOpImpl<ComplexNumber, AddOp<ComplexNumber>>;

template class BinOpImpl<RealNumber, SubOp<RealNumber>>;
template class BinOpImpl<ComplexNumber, SubOp<ComplexNumber>>;

template class BinOpImpl<RealNumber, MulOp<RealNumber>>;
template class BinOpImpl<ComplexNumber, MulOp<ComplexNumber>>;

template class BinOpImpl<RealNumber, DivOp<RealNumber>>;
template class BinOpImpl<ComplexNumber, DivOp<ComplexNumber>>;

template class BinOpImpl<RealNumber, PowOp<RealNumber>>;
template class BinOpImpl<ComplexNumber, PowOp<ComplexNumber>>;
