#include "function.hpp"
#include "mips.hpp"
#include "ast.hpp"
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

int Function::getRegister(std::string &identifier)
{
    // Tenta buscar no contexto local
    auto it = registers.find(identifier);
    if (it != registers.end())
    {
        return it->second;
    }

    // Caso não encontrado no contexto local, busca no contexto global
    auto globalIt = Ast::variables.find(identifier);
    if (globalIt != Ast::variables.end())
    {
        int temp = MIPS::getTemporaryRegister();
        MIPS::loadWord(globalIt->first, temp);
        return temp;
    }

    // Se o identificador não foi encontrado em nenhum dos contextos, lança um erro
    throw std::runtime_error("Identifier '" + identifier + "' not found in local or global context.");
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
