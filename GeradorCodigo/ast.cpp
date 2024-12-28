#include "ast.hpp"

std::unordered_map<std::string, std::unique_ptr<Variable>> Ast::variables;
std::unordered_map<std::string, std::unique_ptr<Function>> Ast::functions;

void Ast::addVariable(std::unique_ptr<Variable> var)
{
    variables[var->getIdentifier()] = std::move(var);
}

void Ast::addFunction(std::unique_ptr<Function> func)
{
    functions[func->getIdentifier()] = std::move(func);
}

void Ast::translate()
{
}
