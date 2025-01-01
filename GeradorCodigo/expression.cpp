#include "expression.hpp"
#include "mips.hpp"
#include "function.hpp"
#include <stdexcept>

Expression::Expression(const char val)
    : value(val), operatorSymbol(OperatorType::NONE), type(ExpressionType::CHARACTER) {}

Expression::Expression(const int val)
    : value(val), operatorSymbol(OperatorType::NONE), type(ExpressionType::INTEGER) {}

Expression::Expression(const std::string &val, ExpressionType type)
    : value(val), operatorSymbol(OperatorType::NONE), type(type) {}

Expression::Expression(const std::string &identifier, std::unique_ptr<std::deque<std::unique_ptr<Expression>>> parameters)
    : parameters(std::move(parameters)), value(identifier),
      operatorSymbol(OperatorType::NONE), type(ExpressionType::FUNCTION_CALL) {}

Expression::Expression(const std::string &identifier, std::unique_ptr<Expression> index)
    : left(std::move(index)), value(identifier),
      operatorSymbol(OperatorType::NONE), type(ExpressionType::ARRAY_ACCESS) {}

Expression::Expression(std::unique_ptr<Expression> child, const OperatorType symbol)
    : left(std::move(child)),
      operatorSymbol(symbol), type(ExpressionType::UNARY) {}

Expression::Expression(std::unique_ptr<Expression> l, std::unique_ptr<Expression> r, const OperatorType symbol)
    : left(std::move(l)), right(std::move(r)),
      operatorSymbol(symbol), type(ExpressionType::BINARY) {}

Expression::Expression(std::unique_ptr<Expression> condition, std::unique_ptr<Expression> true_expr, std::unique_ptr<Expression> false_expr)
    : condition(std::move(condition)), left(std::move(true_expr)), right(std::move(false_expr)),
      operatorSymbol(OperatorType::TERNARY), type(ExpressionType::TERNARY) {}

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

int Expression::translate(Function *func_context, bool reverse, const std::optional<std::string> &jumpTo, int useRegister)
{
    // Registrador de resultado
    int result = (useRegister != -1) ? useRegister : MIPS::getTemporaryRegister();
    switch (type)
    {
    case ExpressionType::TERNARY:
    {
        // Operação ternária
        MIPS::startTernary();
        int r1 = condition->translate(func_context);
        MIPS::branchEqualZero(r1, MIPS::getElseTernary());
        MIPS::freeTemporaryRegister(r1);

        // True Expression
        left->translate(func_context, false, std::nullopt, result);
        MIPS::jumpTo(MIPS::getEndTernary());
        MIPS::startElseTernary();

        // False Expression
        right->translate(func_context, false, std::nullopt, result);
        MIPS::endTernary();
    }
    break;
    case ExpressionType::BINARY:
    {
        // Operação binária
        int r1 = left->translate(func_context);
        int r2 = right->translate(func_context);
        MIPS::createExpression(operatorSymbol, r1, r2, result);
        MIPS::freeTemporaryRegister(r1);
        MIPS::freeTemporaryRegister(r2);
    }
    break;
    case ExpressionType::UNARY:
    {
        // Operação unária
        left->translate(func_context, false, std::nullopt, result);
        MIPS::createExpression(operatorSymbol, result, result);
    }
    break;
    case ExpressionType::ARRAY_ACCESS:
    {
        // Acesso a índices de array
        int rg_index = left->translate(func_context);
        MIPS::createArrayAccess(std::get<std::string>(value.value()), rg_index, result);
    }
    break;
    case ExpressionType::FUNCTION_CALL:
    {
        // Chamada de função
        MIPS::freeTemporaryRegister(result);
        for (auto &param : *parameters)
        {
            int r1 = param->translate(func_context);
            MIPS::moveTo(r1, MIPS::getArgumentRegister());
            MIPS::freeTemporaryRegister(r1);
        }
        MIPS::callFunction(std::get<std::string>(value.value()));
        return RETURN_REGISTER;
    }
    case ExpressionType::INTEGER:
    {
        // Valor inteiro constante
        int intVal = std::get<int>(value.value());
        MIPS::initializeConstant(result, intVal);
    }
    break;
    case ExpressionType::CHARACTER:
    {
        // Valor de caractere constante
        char charVal = std::get<char>(value.value());
        MIPS::initializeConstant(result, static_cast<int>(charVal));
    }
    break;
    case ExpressionType::STRING_LITERAL:
    {
        // String literal
        const std::string &strVal = std::get<std::string>(value.value());
        const std::string &label = MIPS::createString(strVal);
        MIPS::getAddress(result, label);
    }
    break;
    case ExpressionType::IDENTIFIER:
    {
        // Identificador
        MIPS::freeTemporaryRegister(result);
        const int rg = func_context->getRegister(std::get<std::string>(value.value()));
        return rg;
    }
    break;
    }

    // Tratamento de jumps para expressões condicionais
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
