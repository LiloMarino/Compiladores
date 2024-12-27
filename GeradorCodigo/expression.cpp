#include "expression.hpp"
#include <stdexcept>

Expression::Expression(const char val)
    : left(nullptr), right(nullptr), parameters(nullptr), value(val), operatorSymbol(OperatorType::NONE) {}

Expression::Expression(const int val)
    : left(nullptr), right(nullptr), parameters(nullptr), value(val), operatorSymbol(OperatorType::NONE) {}

Expression::Expression(const std::string &val)
    : left(nullptr), right(nullptr), parameters(nullptr), value(val), operatorSymbol(OperatorType::NONE) {}

Expression::Expression(const std::string &identifier, std::unique_ptr<std::deque<std::unique_ptr<Expression>>> parameters)
    : left(nullptr), right(nullptr), parameters(std::move(parameters)), value(identifier),
      operatorSymbol(OperatorType::NONE) {}

Expression::Expression(const std::string &identifier, std::unique_ptr<Expression> index)
    : left(std::move(index)), right(nullptr), parameters(nullptr), value(identifier),
      operatorSymbol(OperatorType::NONE) {}

Expression::Expression(std::unique_ptr<Expression> child, const OperatorType symbol)
    : left(std::move(child)), right(nullptr), parameters(nullptr), value(std::nullopt), operatorSymbol(symbol) {}

Expression::Expression(std::unique_ptr<Expression> l, std::unique_ptr<Expression> r, const OperatorType symbol)
    : left(std::move(l)), right(std::move(r)), parameters(nullptr), value(std::nullopt), operatorSymbol(symbol) {}

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
