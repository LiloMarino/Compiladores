#include "ast.hpp"
#include "mips.hpp"

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
    MIPS mips;
    for (auto& var : variables)
    {
        var.second->translate();
    }
    for (auto& func : functions)
    {
        func.second->translate();
    }
    mips.printCode();
}
