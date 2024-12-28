#include "function.hpp"

Function::Function(const std::string &identifier, std::unique_ptr<Type> return_type,
                   std::unique_ptr<std::deque<std::unique_ptr<Variable>>> parameters,
                   std::unique_ptr<std::deque<std::unique_ptr<Variable>>> variables,
                   std::unique_ptr<std::deque<std::unique_ptr<Command>>> commands)
    : identifier(identifier), return_type(std::move(return_type)), parameters(std::move(parameters)),
      variables(std::move(variables)), commands(std::move(commands)) {}

std::string Function::getIdentifier() const
{
    return identifier;
}
