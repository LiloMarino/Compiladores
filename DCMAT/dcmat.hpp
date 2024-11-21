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
    Function(std::function<double(double)> op, const std::string &symbol = "");

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

class DCMAT
{
private:
    static const int WIDTH = 80;
    static const int HEIGHT = 25;
    std::unique_ptr<Function> last_function = nullptr;
    char graph_matrix[HEIGHT][WIDTH];

public:
    /**
     * @brief Configurações
     */
    Settings settings;

    /**
     * @brief Construtor
     */
    DCMAT();

    /**
     * @brief Seta a última função plotada
     * @param func Função
     */
    void setLastFunction(std::unique_ptr<Function> func);

    /**
     * @brief Plota o gráfico
     * @param func Função a ser plotada
     */
    void plot();

    /**
     * @brief Calcula o valor numérico da integral de uma função usando o método da Soma de Riemann à esquerda
     * @param interval Intervalo de integração
     * @param f Função a ser integrada
     * @return Valor da integral
     */
    double integrate(std::pair<double, double> interval, const Function &f);

private:
    /**
     * @brief Adiciona os eixos ao gráfico
     */
    void drawAxis();

    /**
     * @brief Limpa a matriz com espaços vazios
     */
    void clearGraph();

    /**
     * @brief Imprime o gráfico
     */
    void renderGraph() const;
};

extern DCMAT dcmat;

#endif