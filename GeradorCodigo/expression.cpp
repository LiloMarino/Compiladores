#include "expression.hpp"
#include "mips.hpp"
#include "function.hpp"
#include "ast.hpp"
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
        int r1, r2;
        switch (operatorSymbol)
        {
        case OperatorType::ASSIGN:
        {
            std::string identifier = std::get<std::string>(left->getValue().value());
            r1 = func_context->getRegister(identifier);
            r2 = right->translate(func_context);

            // Primeiro, verificamos se é uma variável local
            if (r1 != -1)
            {
                const Variable *var = func_context->getVariable(identifier); // Variável local
                int pointerLvl = var->getType().getPointerLevel();
                if (pointerLvl > 0)
                {
                    // Variável Local (Array)
                    int rg_index = left->left->translate(func_context); // Traduz índice do array
                    MIPS::createArraySave(identifier, rg_index, r2);
                    MIPS::freeTemporaryRegister(rg_index);
                }
                else
                {
                    // Variável Local (Simples)
                    MIPS::moveTo(r2, r1);
                    MIPS::freeTemporaryRegister(r2);
                    MIPS::freeTemporaryRegister(result);
                    return r1;
                }
            }
            else
            {
                // Caso contrário, buscamos no escopo global
                const Variable *var = Ast::getVariable(identifier);
                int pointerLvl = var->getType().getPointerLevel();
                if (pointerLvl > 0)
                {
                    // Variável Global (Array)
                    int rg_index = left->left->translate(func_context); // Traduz índice do array
                    MIPS::createArraySave(identifier, rg_index, r2);
                    MIPS::freeTemporaryRegister(rg_index);
                }
                else
                {
                    // Variável Global (Simples)
                    MIPS::saveWord(identifier, r2);
                }
            }

            MIPS::freeTemporaryRegister(r2);
            return result;
        }
        break;
        case OperatorType::ADD_ASSIGN:
        case OperatorType::MINUS_ASSIGN:
        {
            std::string identifier = std::get<std::string>(left->getValue().value());
            r1 = func_context->getRegister(identifier);
            r2 = right->translate(func_context);

            OperatorType operation = (operatorSymbol == OperatorType::ADD_ASSIGN) ? OperatorType::PLUS : OperatorType::MINUS;

            // Primeiro, verificamos se é uma variável local
            if (r1 != -1)
            {
                const Variable *var = func_context->getVariable(identifier); // Variável local
                int pointerLvl = var->getType().getPointerLevel();
                if (pointerLvl > 0)
                {
                    // Variável Local (Array)
                    int rg_index = left->left->translate(func_context); // Traduz índice do array
                    int rg_value = MIPS::getTemporaryRegister();
                    MIPS::createArrayAccess(identifier, rg_index, rg_value);   // Carrega valor atual do array
                    MIPS::createExpression(operation, rg_value, r2, rg_value); // Realiza operação
                    MIPS::createArraySave(identifier, rg_index, rg_value);     // Salva resultado
                    MIPS::freeTemporaryRegister(rg_index);
                    MIPS::freeTemporaryRegister(rg_value);
                }
                else
                {
                    // Variável Local (Simples)
                    MIPS::createExpression(operation, r1, r2, r1);
                }
            }
            else
            {
                // Caso contrário, buscamos no escopo global
                const Variable *var = Ast::getVariable(identifier);
                int pointerLvl = var->getType().getPointerLevel();
                if (pointerLvl > 0)
                {
                    // Variável Global (Array)
                    int rg_index = left->left->translate(func_context); // Traduz índice do array
                    int rg_value = MIPS::getTemporaryRegister();
                    MIPS::createArrayAccess(identifier, rg_index, rg_value);   // Carrega valor atual do array
                    MIPS::createExpression(operation, rg_value, r2, rg_value); // Realiza operação
                    MIPS::createArraySave(identifier, rg_index, rg_value);     // Salva resultado
                    MIPS::freeTemporaryRegister(rg_index);
                    MIPS::freeTemporaryRegister(rg_value);
                }
                else
                {
                    // Variável Global (Simples)
                    handleGlobalVariable(operation, identifier, r2);
                }
            }

            MIPS::freeTemporaryRegister(r2);
            return result;
        }
        break;
        default:
        {
            r1 = left->translate(func_context);
            r2 = right->translate(func_context);
            MIPS::createExpression(operatorSymbol, r1, r2, result);
        }
        break;
        }
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
        MIPS::freeTemporaryRegister(rg_index);
    }
    break;
    case ExpressionType::FUNCTION_CALL:
    {
        // Chamada de função
        MIPS::freeTemporaryRegister(result);

        // Preserva registradores de argumento se estiverem em uso
        std::vector<std::function<void()>> restoreActions; // Ações para restaurar depois do jal
        for (size_t i = 0; i < parameters->size(); ++i)
        {
            int arg_rg = MIPS::getRegisterIndex("$a" + std::to_string(i));
            auto &param = (*parameters)[i];

            if (MIPS::isRegisterInUse(arg_rg))
            {
                // Preserva o registrador antes de traduzir o parâmetro
                MIPS::addInstruction("addi $sp, $sp, -4");
                MIPS::addInstruction("sw " + MIPS::getRegisterName(arg_rg) + ", 0($sp)");

                // Adiciona uma ação para restaurar o registrador depois do `jal`
                restoreActions.push_back(
                    [arg_rg, this]()
                    {
                        MIPS::addInstruction("lw " + MIPS::getRegisterName(arg_rg) + ", 0($sp)");
                        MIPS::addInstruction("addi $sp, $sp, 4");
                    });
            }

            // Traduz o parâmetro diretamente para o registrador
            param->translate(func_context, false, std::nullopt, arg_rg);
        }

        // Chama a função
        MIPS::callFunction(std::get<std::string>(value.value()));

        // Restaura os registradores salvos na pilha
        for (auto &restore : restoreActions)
        {
            restore();
        }

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
        const std::string &identifier = std::get<std::string>(value.value());
        int rg = func_context->getRegister(identifier);
        if (rg == -1)
        {
            // Caso não encontrado no contexto local, busca no contexto global
            rg = Ast::getRegister(identifier);
            if (rg == -1)
            {
                // Se o identificador não foi encontrado em nenhum dos contextos, lança um erro
                throw std::runtime_error("Identifier '" + identifier + "' not found in local or global context.");
            }
        }

        if (useRegister != -1)
        {
            MIPS::moveTo(rg, useRegister);
            return useRegister;
        }
        else
        {
            MIPS::freeTemporaryRegister(result);
            if (jumpTo.has_value())
            {
                // Registrador = 1 True, = 0 False
                if (reverse)
                {
                    // Se for falso então jump
                    MIPS::branchEqualZero(rg, jumpTo.value());
                }
                else
                {
                    // Se for true então jump
                    MIPS::branchNotEqualZero(rg, jumpTo.value());
                }
            }
            return rg;
        }
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

void Expression::handleGlobalVariable(OperatorType op, const std::string &identifier, int r2)
{
    int temp = MIPS::getTemporaryRegister();
    MIPS::loadWord(identifier, temp);
    MIPS::createExpression(op, temp, r2, temp);
    MIPS::saveWord(identifier, temp);
    MIPS::freeTemporaryRegister(temp);
}
