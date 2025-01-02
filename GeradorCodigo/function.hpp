#ifndef FUNCTION_HPP
#define FUNCTION_HPP

#include "type.hpp"
#include "variable.hpp"
#include "command.hpp"
#include <unordered_map>

class Function
{
private:
    std::string identifier;
    std::unique_ptr<Type> return_type;
    std::unique_ptr<std::deque<std::unique_ptr<Variable>>> parameters;
    std::unique_ptr<std::deque<std::unique_ptr<Variable>>> variables;
    std::unique_ptr<std::deque<std::unique_ptr<Command>>> commands;
    std::unordered_map<std::string, int> registers;

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

    /**
     * @brief Associa uma varíavel a um registrador
     * @param identifier Identificador da variável
     * @param rg Registrador associado
     */
    void addRegister(std::string& identifier, int rg);

    /**
     * @brief Obtém o registrador associado a uma variável
     * @param identifier Identificador da variável
     * @return Registrador associado
     */
    int getRegister(const std::string& identifier);

    /**
     * @brief Obtém a variável local
     * @param identifier Identificador da variável
     * @return Variável local
     */
    const Variable *getVariable(const std::string &identifier);

    /**
     * @brief Obtém o identificador
     * @return Identificador da função
     */
    std::string getIdentifier() const;

    /**
     * @brief Traduz a Função para código MIPS
     */
    void translate();
};

#endif