#include "function.hpp"
#include <iostream>

Function::Function(std::function<double(double)> op, const std::string &symbol)
    : unaryOperation(op), binaryOperation(nullptr), left(nullptr), right(nullptr), operatorSymbol(symbol) {}

Function::Function(std::function<double(double)> op, std::unique_ptr<Function> child, const std::string &symbol)
    : unaryOperation(op), binaryOperation(nullptr), left(std::move(child)), right(nullptr), operatorSymbol(symbol) {}

Function::Function(std::function<double(double, double)> op, std::unique_ptr<Function> l, std::unique_ptr<Function> r, const std::string &symbol)
    : unaryOperation(nullptr), binaryOperation(op), left(std::move(l)), right(std::move(r)), operatorSymbol(symbol) {}

double Function::operator()(double x) const
{
    if (binaryOperation && left && right)
    {
        // Nó binário
        double leftVal = (*left)(x);
        double rightVal = (*right)(x);
        return binaryOperation(leftVal, rightVal);
    }
    if (unaryOperation && left)
    {
        // Nó unário
        double leftVal = (*left)(x);
        return unaryOperation(leftVal);
    }
    if (unaryOperation)
    {
        // Nó folha (variáveis ou constantes)
        return unaryOperation(x);
    }
    throw std::runtime_error("Árvore de funções malformada.");
}

std::string Function::toRPN() const
{
    // Se for uma operação binária
    if (binaryOperation && left && right)
    {
        return left->toRPN() + " " + right->toRPN() + " " + operatorSymbol;
    }

    // Se for uma operação unária
    if (unaryOperation && left)
    {
        return left->toRPN() + " " + operatorSymbol;
    }

    // Se for uma constante ou variável
    if (!operatorSymbol.empty())
    {
        return operatorSymbol;
    }

    throw std::runtime_error("Árvore de função malformada.");
}