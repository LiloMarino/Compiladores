#ifndef AST_HPP
#define AST_HPP

#include <unordered_map>
#include "variable.hpp"
#include "function.hpp"

class Ast
{
private:
    std::unordered_map<std::string, std::unique_ptr<Variable>> variables;
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
     * @brief Traduz a AST para código MIPS
     */
    void translate();
};

#endif