#ifndef FUNCTION_HPP
#define FUNCTION_HPP

#include <string>
#include <functional>
#include <memory>

/**
 * @brief Classe das funções
 */
class Function
{
private:
    std::function<double(double)> unaryOperation;          // Para valores ou operações unárias
    std::function<double(double, double)> binaryOperation; // Para operações binárias
    std::unique_ptr<Function> left;                        // Subárvore esquerda
    std::unique_ptr<Function> right;                       // Subárvore direita
    std::string operatorSymbol;                            // Representação do operador

public:
    /**
     * @brief Construtor para valores base (folhas)
     * @param op Função lambda
     */
    Function(std::function<double(double)> op, const std::string &symbol);

    /**
     * @brief Construtor para operações unárias
     * @param op Função lambda
     * @param child Subárvore
     */
    Function(std::function<double(double)> op, std::unique_ptr<Function> child, const std::string &symbol);

    /**
     * @brief Construtor para operações binárias
     * @param op Função lambda
     * @param left Função à esquerda
     * @param right Função à direita
     */
    Function(std::function<double(double, double)> op, std::unique_ptr<Function> left, std::unique_ptr<Function> right, const std::string &symbol);

    /**
     * @brief Torna a classe callable, aplicando todas as funções na ordem correta
     * @param x Valor de entrada
     * @return Retorna o valor da função f(x) aplicada em x
     */
    double operator()(double x) const;

    /**
     * @brief Converte a expressão para RPN (Reverse Polish Notation)
     * @return Retorna a expressão RPN
     */
    std::string toRPN() const;
};

#endif