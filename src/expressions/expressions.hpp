#ifndef EXPRESSIONS_HPP
#define EXPRESSIONS_HPP

#include <memory>
#include <string>
#include <unordered_map>

enum class OpPrecedence {
    AddSub = 0,
    Mul = 1,
    Div = 2,
    Pow = 3
};

template<typename T>
class BaseExpr {
public:
    virtual std::shared_ptr<BaseExpr> with_values(
        const std::unordered_map<std::string, T>& values
    ) const = 0;

    virtual T resolve() const = 0;
    virtual std::shared_ptr<BaseExpr> diff(const std::string& by) const = 0;
    virtual std::string to_string() const = 0;

protected:
    BaseExpr() = default;
    virtual ~BaseExpr() = default;
};

template<typename T>
class Expression {
public:
    explicit Expression(T number);
    explicit Expression(const std::string& var_name);

    Expression sin() const;
    Expression cos() const;
    Expression ln() const;
    Expression exp() const;

    Expression& operator=(const Expression& rhs);

    Expression operator+(const Expression& rhs) const;
    Expression& operator+=(const Expression& rhs);

    Expression operator-(const Expression& rhs) const;
    Expression& operator-=(const Expression& rhs);

    Expression operator*(const Expression& rhs) const;
    Expression& operator*=(const Expression& rhs);

    Expression operator/(const Expression& rhs) const;
    Expression& operator/=(const Expression& rhs);

    Expression operator^(const Expression& rhs) const;
    Expression& operator^=(const Expression& rhs);

    Expression with_values(std::unordered_map<std::string, T>& values) const;

    T resolve() const;
    T resolve_with(std::unordered_map<std::string, T>& values) const;

    Expression diff(const std::string& by) const;

    std::string to_string() const;

private:
    std::shared_ptr<BaseExpr<T>> inner;

    explicit Expression(std::shared_ptr<BaseExpr<T>> expression_impl);
};

template<typename T>
class Constant final : public BaseExpr<T> {
public:
    explicit Constant(T _value);

    std::shared_ptr<BaseExpr<T>> with_values(
        const std::unordered_map<std::string, T>& values
    ) const override;

    T resolve() const override;
    std::shared_ptr<BaseExpr<T>> diff(const std::string& by) const override;
    std::string to_string() const override;

private:
    T value;
};

template<typename T>
class Variable final : public BaseExpr<T> {
public:
    explicit Variable(std::string _name);

    std::shared_ptr<BaseExpr<T>> with_values(
        const std::unordered_map<std::string, T>& values
    ) const override;

    T resolve() const override;
    std::shared_ptr<BaseExpr<T>> diff(const std::string& by) const override;
    std::string to_string() const override;

private:
    std::string name;
};

template<typename T>
class BinOp : public BaseExpr<T> {
public:
    BinOp(
       const std::shared_ptr<BaseExpr<T>>& _lhs,
       const std::shared_ptr<BaseExpr<T>>& _rhs
    );

    virtual std::string name() const = 0;
    virtual OpPrecedence precedence() const = 0;

protected:
    std::shared_ptr<BaseExpr<T>> lhs;
    std::shared_ptr<BaseExpr<T>> rhs;
};

template<typename T, typename Derived>
class BinOpImpl : public BinOp<T> {
public:
    using BinOp<T>::BinOp;

    std::shared_ptr<BaseExpr<T>> with_values(
        const std::unordered_map<std::string, T>& values
    ) const override;

    std::string to_string() const override;
};

template<typename T>
class Func : public BaseExpr<T> {
public:
    explicit Func(const std::shared_ptr<BaseExpr<T>>& _argument);

    virtual std::string name() const = 0;

protected:
    std::shared_ptr<BaseExpr<T>> argument;
};

template<typename T, typename Derived>
class FuncImpl : public Func<T> {
public:
    using Func<T>::Func;

    std::shared_ptr<BaseExpr<T>> with_values(
        const std::unordered_map<std::string, T>& values
    ) const override;

    std::string to_string() const override;
};

template<typename T>
class AddOp final : public BinOpImpl<T, AddOp<T>> {
public:
    using BinOpImpl<T, AddOp>::BinOpImpl;

    T resolve() const override;
    std::shared_ptr<BaseExpr<T>> diff(const std::string& by) const override;

private:
    std::string name() const override {
        return "+";
    }

    OpPrecedence precedence() const override {
        return OpPrecedence::AddSub;
    }
};

template<typename T>
class SubOp final : public BinOpImpl<T, SubOp<T>> {
public:
    using BinOpImpl<T, SubOp>::BinOpImpl;

    T resolve() const override;
    std::shared_ptr<BaseExpr<T>> diff(const std::string& by) const override;

private:
    std::string name() const override {
        return "-";
    }

    OpPrecedence precedence() const override {
        return OpPrecedence::AddSub;
    }
};

template<typename T>
class MulOp final : public BinOpImpl<T, MulOp<T>> {
public:
    using BinOpImpl<T, MulOp>::BinOpImpl;

    T resolve() const override;
    std::shared_ptr<BaseExpr<T>> diff(const std::string& by) const override;

private:
    std::string name() const override {
        return "*";
    }

    OpPrecedence precedence() const override {
        return OpPrecedence::Mul;
    }
};

template<typename T>
class DivOp final : public BinOpImpl<T, DivOp<T>> {
public:
    using BinOpImpl<T, DivOp>::BinOpImpl;

    T resolve() const override;
    std::shared_ptr<BaseExpr<T>> diff(const std::string& by) const override;

private:
    std::string name() const override {
        return "/";
    }

    OpPrecedence precedence() const override {
        return OpPrecedence::Div;
    }
};

template<typename T>
class PowOp final : public BinOpImpl<T, PowOp<T>> {
public:
    using BinOpImpl<T, PowOp>::BinOpImpl;

    T resolve() const override;
    std::shared_ptr<BaseExpr<T>> diff(const std::string& by) const override;

private:
    std::string name() const override {
        return "^";
    }

    OpPrecedence precedence() const override {
        return OpPrecedence::Pow;
    }
};

template<typename T>
class SinFunc final : public FuncImpl<T, SinFunc<T>> {
public:
    using FuncImpl<T, SinFunc>::FuncImpl;

    T resolve() const override;
    std::shared_ptr<BaseExpr<T>> diff(const std::string& by) const override;

private:
    std::string name() const override {
        return "sin";
    }
};

template<typename T>
class CosFunc final : public FuncImpl<T, CosFunc<T>> {
public:
    using FuncImpl<T, CosFunc>::FuncImpl;

    T resolve() const override;
    std::shared_ptr<BaseExpr<T>> diff(const std::string& by) const override;

private:
    std::string name() const override {
        return "cos";
    }
};

template<typename T>
class LnFunc final : public FuncImpl<T, LnFunc<T>> {
public:
    using FuncImpl<T, LnFunc>::FuncImpl;

    T resolve() const override;
    std::shared_ptr<BaseExpr<T>> diff(const std::string& by) const override;

private:
    constexpr std::string name() const override {
        return "ln";
    }
};

template<typename T>
class ExpFunc final : public FuncImpl<T, ExpFunc<T>> {
public:
    using FuncImpl<T, ExpFunc>::FuncImpl;

    T resolve() const override;
    std::shared_ptr<BaseExpr<T>> diff(const std::string& by) const override;

private:
    std::string name() const override {
        return "exp";
    }
};

#endif  // EXPRESSIONS_HPP
