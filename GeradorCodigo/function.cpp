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

const Variable *Function::getVariable(const std::string &identifier)
{
    // Itera sobre a deque de variáveis para procurar a variável com o identificador dado
    for (const auto &variable : *variables)
    {
        if (variable->getIdentifier() == identifier)
        {
            return variable.get();
        }
    }

    // Se não encontrar, procura nos parâmetros
    for (const auto &parameter : *parameters)
    {
        if (parameter->getIdentifier() == identifier)
        {
            return parameter.get();
        }
    }

    // Caso a variável não seja encontrada, lança uma exceção
    throw std::runtime_error("Variável não encontrada: " + identifier);
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
    if (!commands->empty())
    {
        // Acessa o último comando
        auto lastCommand = commands->back().get();

        // Verifica se é um comando do tipo Return
        if (!(lastCommand->getType() == CommandType::RETURN))
        {
            MIPS::callReturn(-1);
        }
    }
    MIPS::freeAllRegisters();
}
