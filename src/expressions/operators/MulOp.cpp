#include "../expressions.hpp"

template<typename T>
T MulOp<T>::resolve() const {
    return this->lhs->resolve() * this->rhs->resolve();
}

template<typename T>
std::shared_ptr<BaseExpr<T>> MulOp<T>::diff(const std::string& by) const {
    return std::make_shared<AddOp<T>>(
        std::make_shared<MulOp<T>>(
            this->lhs->diff(by),
            this->rhs
        ),
        std::make_shared<MulOp<T>>(
            this->lhs,
            this->rhs->diff(by)
        )
    );
}

template class MulOp<RealNumber>;
template class MulOp<ComplexNumber>;
