#ifndef AST_HPP
#define AST_HPP

#include <unordered_map>
#include "variable.hpp"
#include "function.hpp"

class Ast
{
private:
    static std::unordered_map<std::string, std::unique_ptr<Variable>> variables;
    std::unordered_map<std::string, std::unique_ptr<Function>> functions;

public:
    /**
     * @brief Adiciona uma variável a AST
     * @param var Variável
     */
    void addVariable(std::unique_ptr<Variable> var);

    /**
     * @brief Adiciona uma função a AST
     * @param func Função
     */
    void addFunction(std::unique_ptr<Function> func);

    /**
     * @brief Obtém o registrador associado a uma variável
     * @param identifier Identificador da variável
     * @return Registrador associado
     */
    static int getRegister(const std::string &identifier);

    /**
     * @brief Traduz a AST para código MIPS
     */
    void translate();
};

#endif