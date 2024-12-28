#ifndef FUNCTION_HPP
#define FUNCTION_HPP

#include "type.hpp"
#include "variable.hpp"
#include "command.hpp"

class Function
{
private:
    std::string identifier;
    std::unique_ptr<Type> return_type;
    std::unique_ptr<std::deque<std::unique_ptr<Variable>>> parameters;
    std::unique_ptr<std::deque<std::unique_ptr<Variable>>> variables;
    std::unique_ptr<std::deque<std::unique_ptr<Command>>> commands;

public:
    /**
     * @brief Construtor de uma função
     * @param identifier Identificador da função
     * @param return_type Tipo de retorno da função
     * @param parameters Lista de parâmetros da função
     * @param variables Lista de variáveis da função
     * @param commands Lista de comandos da função
     */
    Function(const std::string &identifier, std::unique_ptr<Type> return_type,
             std::unique_ptr<std::deque<std::unique_ptr<Variable>>> parameters,
             std::unique_ptr<std::deque<std ::unique_ptr<Variable>>> variables,
             std::unique_ptr<std::deque<std::unique_ptr<Command>>> commands);
};

#endif