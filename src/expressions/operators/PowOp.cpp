#include "../expressions.hpp"

template<typename T>
T PowOp<T>::resolve() const {
    return std::pow(this->lhs->resolve(), this->rhs->resolve());
}

template<typename T>
std::shared_ptr<BaseExpr<T>> PowOp<T>::diff(const std::string& by) const {
    return std::make_shared<MulOp<T>>(
        std::make_shared<PowOp<T>>(
            this->lhs,
            this->rhs
        ),
        std::make_shared<AddOp<T>>(
            std::make_shared<DivOp<T>>(
                std::make_shared<MulOp<T>>(
                    this->lhs->diff(by),
                    this->rhs
                ),
                this->lhs
            ),
            std::make_shared<MulOp<T>>(
                std::make_shared<LnFunc<T>>(this->lhs),
                this->rhs->diff(by)
            )
        )
    );
}

template class PowOp<RealNumber>;
template class PowOp<ComplexNumber>;
