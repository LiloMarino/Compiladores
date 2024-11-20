#ifndef DCMAT_HPP
#define DCMAT_HPP

#include <string>
#include <iostream>
#include <functional>
#include <memory>

/**
 * @brief Configurações
 */
struct Settings
{
    double h_view_lo = -6.5;
    double h_view_hi = 6.5;
    double v_view_lo = -3.5;
    double v_view_hi = 3.5;
    int float_precision = 6;
    int integral_steps = 1000;
    bool draw_axis = true;
    bool erase_plot = true;
    bool connect_dots = false;

    /**
     * @brief Método para mostrar as configurações
     */
    void show() const;

    /**
     * @brief Método para resetar as configurações
     */
    void reset();

    /**
     * @brief Seta os parâmetros de visualização horizontal do eixo X
     * @param low Lower Bound
     * @param high Upper Bound
     */
    void setHView(std::pair<double, double> h_view);

    /**
     * @brief Seta os parâmetros de visualização vertical do eixo Y
     * @param low Lower Bound
     * @param high Upper Bound
     */
    void setVView(std::pair<double, double> v_view);
};

/**
 * @brief Classe das funções
 */
class Function
{
private:
    std::function<double(double, double)> binaryOperation; // Para operações binárias
    std::function<double(double)> unaryOperation;          // Para valores ou operações unárias
    std::unique_ptr<Function> left;                        // Subárvore esquerda
    std::unique_ptr<Function> right;                       // Subárvore direita

public:
    /**
     * @brief Construtor para valores base (folhas)
     * @param op Função lambda
     */
    Function(std::function<double(double)> op);

    /**
     * @brief Construtor para operações unárias
     * @param op Função lambda
     * @param child Subárvore
     */
    Function(std::function<double(double)> op, std::unique_ptr<Function> child);

    /**
     * @brief Construtor para operações binárias
     * @param op Função lambda
     * @param left Função à esquerda
     * @param right Função à direita
     */
    Function(std::function<double(double, double)> op, std::unique_ptr<Function> left, std::unique_ptr<Function> right);

    /**
     * @brief Torna a classe callable, aplicando todas as funções na ordem correta
     * @param x Valor de entrada
     * @return Retorna o valor da função f(x) aplicada em x
     */
    double operator()(double x) const;
};

class DCMAT
{
private:
    std::unique_ptr<Function> last_function = nullptr;
public:
    /**
     * @brief Configurações
     */
    Settings settings;

    /**
     * @brief 
     * @param func 
     */
    void setLastFunction(std::unique_ptr<Function> func);

    /**
     * @brief Plota o gráfico
     * @param func Função a ser plotada
     */
    void plot();
};

extern DCMAT dcmat;

#endif