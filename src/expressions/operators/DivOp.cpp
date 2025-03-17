#include "../expressions.hpp"

template<typename T>
T DivOp<T>::resolve() const {
    return this->lhs->resolve() / this->rhs->resolve();
}

template<typename T>
std::shared_ptr<BaseExpr<T>> DivOp<T>::diff(const std::string& by) const {
    return std::make_shared<DivOp<T>>(
        std::make_shared<AddOp<T>>(
            std::make_shared<MulOp<T>>(
                this->lhs->diff(by),
                this->rhs
            ),
            std::make_shared<MulOp<T>>(
                this->lhs,
                this->rhs->diff(by)
            )
        ),
        std::make_shared<PowOp<T>>(
            this->rhs,
            std::make_shared<Constant<T>>(2)
        )
    );
}

template class DivOp<RealNumber>;
template class DivOp<ComplexNumber>;
