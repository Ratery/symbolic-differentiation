#include "../expressions.hpp"

#include <cmath>

template<typename T>
T ExpFunc<T>::resolve() const {
    return std::exp(this->argument->resolve());
}

template<typename T>
std::shared_ptr<BaseExpr<T>> ExpFunc<T>::diff(const std::string& by) const {
    return std::make_shared<MulOp<T>>(
        std::make_shared<ExpFunc>(*this),
        this->argument->diff(by)
    );
}

template class ExpFunc<RealNumber>;
template class ExpFunc<ComplexNumber>;
