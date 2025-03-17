#include "../expressions.hpp"

#include <cmath>

template<typename T>
T CosFunc<T>::resolve() const {
    return std::cos(this->argument->resolve());
}

template<typename T>
std::shared_ptr<BaseExpr<T>> CosFunc<T>::diff(const std::string& by) const {
    return std::make_shared<MulOp<T>>(
        std::make_shared<MulOp<T>>(
            std::make_shared<Constant<T>>(-1),
            std::make_shared<SinFunc<T>>(this->argument)
        ),
        this->argument->diff(by)
    );
}

template class CosFunc<long double>;
