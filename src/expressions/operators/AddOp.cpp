#include "../expressions.hpp"

template<typename T>
T AddOp<T>::resolve() const {
    return this->lhs->resolve() + this->rhs->resolve();
}

template<typename T>
std::shared_ptr<BaseExpr<T>> AddOp<T>::diff(const std::string& by) const {
    return std::make_shared<AddOp>(
        this->lhs->diff(by),
        this->rhs->diff(by)
    );
}

template class AddOp<long double>;
