#include "expression.hpp"
#include "mips.hpp"
#include <stdexcept>

Expression::Expression(const char val)
    : condition(nullptr), left(nullptr), right(nullptr), parameters(nullptr), value(val),
      operatorSymbol(OperatorType::NONE) {}

Expression::Expression(const int val)
    : condition(nullptr), left(nullptr), right(nullptr), parameters(nullptr), value(val),
      operatorSymbol(OperatorType::NONE) {}

Expression::Expression(const std::string &val)
    : condition(nullptr), left(nullptr), right(nullptr), parameters(nullptr), value(val),
      operatorSymbol(OperatorType::NONE) {}

Expression::Expression(const std::string &identifier, std::unique_ptr<std::deque<std::unique_ptr<Expression>>> parameters)
    : condition(nullptr), left(nullptr), right(nullptr), parameters(std::move(parameters)), value(identifier),
      operatorSymbol(OperatorType::NONE) {}

Expression::Expression(const std::string &identifier, std::unique_ptr<Expression> index)
    : condition(nullptr), left(std::move(index)), right(nullptr), parameters(nullptr), value(identifier),
      operatorSymbol(OperatorType::NONE) {}

Expression::Expression(std::unique_ptr<Expression> child, const OperatorType symbol)
    : condition(nullptr), left(std::move(child)), right(nullptr), parameters(nullptr), value(std::nullopt),
      operatorSymbol(symbol) {}

Expression::Expression(std::unique_ptr<Expression> l, std::unique_ptr<Expression> r, const OperatorType symbol)
    : condition(nullptr), left(std::move(l)), right(std::move(r)), parameters(nullptr), value(std::nullopt),
      operatorSymbol(symbol) {}

Expression::Expression(std::unique_ptr<Expression> condition, std::unique_ptr<Expression> true_expr, std::unique_ptr<Expression> false_expr)
    : condition(std::move(condition)), left(std::move(true_expr)), right(std::move(false_expr)), parameters(nullptr),
      value(std::nullopt), operatorSymbol(OperatorType::TERNARY) {}

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

int Expression::translate(bool reverse, const std::optional<std::string> &jumpTo, int useRegister)
{
    // Registrador de resultado
    int result = (useRegister != -1) ? useRegister : MIPS::getTemporaryRegister();
    if (isOperatorNode())
    {
        if (condition)
        {
            // Operação ternária
            MIPS::startTernary();
            int r1 = condition->translate();
            MIPS::branchEqualZero(r1, MIPS::getElseTernary());
            MIPS::freeTemporaryRegister(r1);
            // True Expression
            left->translate(false, std::nullopt, result);
            MIPS::jumpTo(MIPS::getEndTernary());
            MIPS::startElseTernary();
            // False Expression
            right->translate(false, std::nullopt, result);
            MIPS::endTernary();
        }
        else if (right)
        {
            // Operação binária
            int r1 = left->translate();
            int r2 = right->translate();
            MIPS::createExpression(operatorSymbol, r1, r2, result);
            MIPS::freeTemporaryRegister(r1);
            MIPS::freeTemporaryRegister(r2);
        }
        else
        {
            // Operação unária
            left->translate(false, std::nullopt, result);
            MIPS::createExpression(operatorSymbol, result, result);
        }
        if (jumpTo.has_value())
        {
            // Registrador = 1 True, = 0 False
            if (reverse)
            {
                // Se for falso então jump
                MIPS::branchEqualZero(result, jumpTo.value());
            }
            else
            {
                // Se for true então jump
                MIPS::branchNotEqualZero(result, jumpTo.value());
            }
        }
        return result;
    }
    else
    {
        if (left)
        {
            // Acesso a índices de array
            int rg_index = left->translate();
            MIPS::createArrayAcess(std::get<std::string>(value.value()), rg_index, result);
            return result;
        }
        else if (parameters)
        {
            // Chamada de função
            MIPS::freeTemporaryRegister(result);
            for (auto &param : *parameters)
            {
                int r1 = param->translate();
                MIPS::moveTo(r1, MIPS::getArgumentRegister());
                MIPS::freeTemporaryRegister(r1);
            }
            MIPS::callFunction(std::get<std::string>(value.value()));
            return -1;
        }
        else
        {
            // Valor constante
            const auto &val = value.value();
            if (std::holds_alternative<int>(val))
            {
                int intVal = std::get<int>(val);
                MIPS::initializeConstant(result, intVal);
            }
            else if (std::holds_alternative<char>(val))
            {
                char charVal = std::get<char>(val);
                MIPS::initializeConstant(result, static_cast<int>(charVal));
            }
            else if (std::holds_alternative<std::string>(val))
            {
                const std::string &strVal = std::get<std::string>(val);
                const std::string &label = MIPS::createString(strVal);
                MIPS::getAddress(result, label);
            }
            return result;
        }
    }
}
