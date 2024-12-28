#include "command.hpp"

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
