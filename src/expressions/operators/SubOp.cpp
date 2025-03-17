#include "../expressions.hpp"

template<typename T>
T SubOp<T>::resolve() const {
    return this->lhs->resolve() - this->rhs->resolve();
}

template<typename T>
std::shared_ptr<BaseExpr<T>> SubOp<T>::diff(const std::string& by) const {
    return std::make_shared<SubOp<T>>(
        this->lhs->diff(by),
        this->rhs->diff(by)
    );
}

template class SubOp<RealNumber>;
template class SubOp<ComplexNumber>;
