#include "../expressions.hpp"

#include <cmath>

template<typename T>
T LnFunc<T>::resolve() const {
    return std::log(this->argument->resolve());
}

template<typename T>
std::shared_ptr<BaseExpr<T>> LnFunc<T>::diff(const std::string& by) const {
    return std::make_shared<MulOp<T>>(
        std::make_shared<DivOp<T>>(
            std::make_shared<Constant<T>>(1),
            this->argument
        ),
        this->argument->diff(by)
    );
}

template class LnFunc<long double>;
