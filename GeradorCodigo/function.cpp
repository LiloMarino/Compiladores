#include "function.hpp"
#include "mips.hpp"
#include <stdexcept>

Function::Function(const std::string &identifier, std::unique_ptr<Type> return_type,
                   std::unique_ptr<std::deque<std::unique_ptr<Variable>>> parameters,
                   std::unique_ptr<std::deque<std::unique_ptr<Variable>>> variables,
                   std::unique_ptr<std::deque<std::unique_ptr<Command>>> commands)
    : identifier(identifier), return_type(std::move(return_type)), parameters(std::move(parameters)),
      variables(std::move(variables)), commands(std::move(commands)) {}

void Function::addRegister(std::string &identifier, int rg)
{
    registers[identifier] = rg;
}

int Function::getRegister(const std::string &identifier)
{
    auto it = registers.find(identifier);
    if (it != registers.end())
    {
        return it->second;
    }
    return -1;
}


std::string Function::getIdentifier() const
{
    return identifier;
}

void Function::translate()
{
    MIPS::createLabel(identifier);
    for (auto &param : *parameters)
    {
        param->translate(this);
    }
    for (auto &var : *variables)
    {
        var->translate(this);
    }
    for (auto &cmd : *commands)
    {
        cmd->translate(this);
    }
    MIPS::freeAllRegisters();
}
