#include "expression.hpp"
#include <stdexcept>

Expression::Expression(const std::variant<int, char, std::string> &val)
    : left(nullptr), right(nullptr), value(val), operatorSymbol(OperatorType::NONE) {}

Expression::Expression(std::unique_ptr<Expression> child, const OperatorType symbol)
    : left(std::move(child)), right(nullptr), value(std::nullopt), operatorSymbol(symbol) {}

Expression::Expression(std::unique_ptr<Expression> l, std::unique_ptr<Expression> r, const OperatorType symbol)
    : left(std::move(l)), right(std::move(r)), value(std::nullopt), operatorSymbol(symbol) {}

OperatorType Expression::getOperator() const
{
    if (!isOperatorNode())
    {
        throw std::logic_error("This is not an operator node");
    }
    return operatorSymbol;
}

std::optional<std::variant<int, char, std::string>> Expression::getValue() const
{
    return value;
}

bool Expression::isValueNode() const
{
    return value.has_value();
}

bool Expression::isOperatorNode() const
{
    return operatorSymbol != OperatorType::NONE;
}

void Expression::applyPreOrder(const std::function<void(const Expression &)> &func) const
{
    // Aplica a função ao nó atual
    func(*this);

    // Recursivamente aplica à subárvore esquerda
    if (left)
    {
        left->applyPreOrder(func);
    }

    // Recursivamente aplica à subárvore direita
    if (right)
    {
        right->applyPreOrder(func);
    }
}
