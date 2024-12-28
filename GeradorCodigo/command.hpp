#ifndef COMMAND_HPP
#define COMMAND_HPP

#include <memory>
#include <deque>
#include "expression.hpp"

enum class CommandType
{
    DO_WHILE,
    IF,
    IF_ELSE,
    WHILE,
    FOR,
    PRINTF,
    SCANF,
    EXIT,
    RETURN,
    EXPRESSION,
};

class Command
{
private:
    CommandType type;
    std::unique_ptr<Expression> assign = nullptr;
    std::unique_ptr<Expression> condition = nullptr;
    std::unique_ptr<Expression> step = nullptr;
    std::unique_ptr<std::deque<std::unique_ptr<Expression>>> parameters = nullptr;
    std::unique_ptr<std::deque<std::unique_ptr<Command>>> commands = nullptr;
    std::unique_ptr<std::deque<std::unique_ptr<Command>>> second_commands = nullptr;
    std::optional<std::string> string = std::nullopt;

public:
    /**
     * @brief Construtor para IF, WHILE e DO-WHILE
     * @param type Tipo do comando
     * @param condition Condição para o comando
     * @param commands Comandos a serem executados
     */
    Command(CommandType type, std::unique_ptr<Expression> condition,
            std::unique_ptr<std::deque<std::unique_ptr<Command>>> commands);

    /**
     * @brief Construtor para IF-ELSE
     * @param condition Condição para o comando
     * @param if_block Comandos se verdadeiro
     * @param else_block Comandos se falso
     */
    Command(std::unique_ptr<Expression> condition,
            std::unique_ptr<std::deque<std::unique_ptr<Command>>> if_block,
            std::unique_ptr<std::deque<std::unique_ptr<Command>>> else_block);

    /**
     * @brief Construtor para FOR
     * @param assign Atribuição do FOR
     * @param condition Condição do FOR
     * @param step Passo do FOR
     * @param commands Comandos a serem executados
     */
    Command(std::unique_ptr<Expression> assign, std::unique_ptr<Expression> condition,
            std::unique_ptr<Expression> step, std::unique_ptr<std::deque<std::unique_ptr<Command>>> commands);

    /**
     * @brief Construtor para EXIT, RETURN e EXPRESSION
     * @param type Tipo do comando
     * @param string String a ser impressa
     * @param expression Expressão a ser avaliada
     */
    Command(CommandType type, std::unique_ptr<Expression> expression = nullptr);

    /**
     * @brief Construtor para SCANF
     * @param string String a ser lida
     * @param assign Atribuição do SCANF
     */
    Command(const std::optional<std::string> &string, std::unique_ptr<Expression> assign);
    
    /**
     * @brief Construtor para PRINTF
     * @param string String a ser impressa
     * @param parameters Argumentos a serem impressos
     */
    Command(const std::optional<std::string> &string,
            std::unique_ptr<std::deque<std::unique_ptr<Expression>>> parameters);
};

#endif