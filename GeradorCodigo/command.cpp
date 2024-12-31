#include "command.hpp"
#include "mips.hpp"
#include <stdexcept>
#include <algorithm>

Command::Command(CommandType type, std::unique_ptr<Expression> condition,
                 std::unique_ptr<std::deque<std::unique_ptr<Command>>> commands)
    : type(type), condition(std::move(condition)), commands(std::move(commands)) {}

Command::Command(std::unique_ptr<Expression> condition,
                 std::unique_ptr<std::deque<std::unique_ptr<Command>>> if_block,
                 std::unique_ptr<std::deque<std::unique_ptr<Command>>> else_block)
    : type(CommandType::IF_ELSE), condition(std::move(condition)),
      commands(std::move(if_block)), second_commands(std::move(else_block)) {}

Command::Command(std::unique_ptr<Expression> assign, std::unique_ptr<Expression> condition,
                 std::unique_ptr<Expression> step, std::unique_ptr<std::deque<std::unique_ptr<Command>>> commands)
    : type(CommandType::FOR), assign(std::move(assign)), condition(std::move(condition)),
      step(std::move(step)), commands(std::move(commands)) {}

Command::Command(CommandType type, std::unique_ptr<Expression> expression)
    : type(type), assign(std::move(expression)) {}

Command::Command(const std::optional<std::string> &string, std::unique_ptr<Expression> assign)
    : type(CommandType::SCANF), assign(std::move(assign)), string(string) {}

Command::Command(const std::optional<std::string> &string,
                 std::unique_ptr<std::deque<std::unique_ptr<Expression>>> parameters)
    : type(CommandType::PRINTF), parameters(std::move(parameters)), string(string) {}

void Command::translate()
{
    switch (type)
    {
    case CommandType::DO_WHILE:
    {
        std::string label = MIPS::startWhile();
        for (auto &cmd : *commands)
        {
            cmd->translate();
        }
        condition->translate(false, label);
        MIPS::endWhile();
    }
    break;
    case CommandType::IF:
    {
        MIPS::startIf();
        condition->translate(true, MIPS::getEndIf());
        for (auto &cmd : *commands)
        {
            cmd->translate();
        }
        MIPS::endIf();
    }
    break;
    case CommandType::IF_ELSE:
    {
        MIPS::startIf();
        condition->translate(true, MIPS::getElse());
        for (auto &cmd : *commands)
        {
            cmd->translate();
        }
        MIPS::jumpTo(MIPS::getEndIf());
        MIPS::startElse();
        for (auto &cmd : *second_commands)
        {
            cmd->translate();
        }
        MIPS::endIf();
    }
    break;
    case CommandType::WHILE:
    {
        std::string label = MIPS::startWhile();
        condition->translate(true, MIPS::getEndWhile());
        for (auto &cmd : *commands)
        {
            cmd->translate();
        }
        MIPS::jumpTo(label);
        MIPS::endWhile();
    }
    break;
    case CommandType::FOR:
    {
        assign->translate();
        std::string label = MIPS::startFor();
        condition->translate(true, MIPS::getEndFor());
        for (auto &cmd : *commands)
        {
            cmd->translate();
        }
        step->translate();
        MIPS::jumpTo(label);
        MIPS::endFor();
    }
    break;
    case CommandType::PRINTF:
    {
        std::string &format = string.value();
        format.erase(std::remove(format.begin(), format.end(), '"'), format.end());
        size_t pos = 0;
        size_t start = 0;

        if (parameters)
        {
            auto param_iter = parameters->begin();

            // Percorre a string para encontrar '%' seguido de um especificador
            while ((pos = format.find('%', start)) != std::string::npos)
            {
                // Extrai o texto antes do '%'
                if (pos > start)
                {
                    std::string text = format.substr(start, pos - start);
                    MIPS::callPrintf(text);
                }

                char specifier = format[pos + 1];
                switch (specifier)
                {
                case 'd': // Inteiro
                    if (param_iter != parameters->end())
                    {
                        int reg = (*param_iter)->translate();
                        MIPS::callPrintf(reg);
                        ++param_iter;
                    }
                    else
                    {
                        throw std::runtime_error("Not enough parameters for '%d'.");
                    }
                    break;

                case 's': // String
                    if (param_iter != parameters->end())
                    {
                        int reg = (*param_iter)->translate();
                        MIPS::callPrintfAsString(reg);
                        ++param_iter;
                    }
                    else
                    {
                        throw std::runtime_error("Not enough parameters for '%s'.");
                    }
                    break;

                case 'c': // Caractere
                    if (param_iter != parameters->end())
                    {
                        int reg = (*param_iter)->translate();
                        MIPS::callPrintfAsChar(reg);
                        ++param_iter;
                    }
                    else
                    {
                        throw std::runtime_error("Not enough parameters for '%c'.");
                    }
                    break;

                default:
                    throw std::runtime_error("Unsupported format specifier: %" + std::string(1, specifier));
                }
                // Avança para o próximo trecho da string
                start = pos + 2;
            }
        }

        // Imprime o restante da string, se houver
        if (start < format.size())
        {
            std::string remaining = format.substr(start);
            MIPS::callPrintf(remaining);
        }
    }
    break;
    case CommandType::SCANF:
    {
        const std::string &format = string.value();
        size_t pos = 0;
        if ((pos = format.find('%', 0)) != std::string::npos)
        {
            char specifier = format[pos + 1];
            switch (specifier)
            {
            case 'd': // Inteiro
            {
                const auto &var = assign->getValue().value();
                MIPS::callScanf(std::get<std::string>(var));
            }
            break;
            default:
                throw std::runtime_error("Unsupported format specifier: %" + std::string(1, specifier));
            }
        }
    }
    break;
    case CommandType::EXIT:
    {
        int rg = assign->translate();
        MIPS::callExit(rg);
    }
    break;
    case CommandType::RETURN:
    {
        if (assign)
        {
            int rg = assign->translate();
            MIPS::callReturn(rg);
        }
        else
        {
            MIPS::callReturn(-1);
        }
    }
    break;
    case CommandType::EXPRESSION:
    {
        assign->translate();
    }
    break;
    }
}
