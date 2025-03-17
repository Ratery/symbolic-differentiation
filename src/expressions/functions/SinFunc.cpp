#include "../expressions.hpp"

#include <cmath>

template<typename T>
T SinFunc<T>::resolve() const {
    return std::sin(this->argument->resolve());
}

template<typename T>
std::shared_ptr<BaseExpr<T>> SinFunc<T>::diff(const std::string& by) const {
    return std::make_shared<MulOp<T>>(
        std::make_shared<CosFunc<T>>(this->argument),
        this->argument->diff(by)
    );
}

template class SinFunc<long double>;
