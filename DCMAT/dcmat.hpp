#ifndef DCMAT_HPP
#define DCMAT_HPP

#include <string>
#include <iostream>
#include <functional>

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
    void setHView(std::pair <double, double> h_view);

    /**
     * @brief Seta os parâmetros de visualização vertical do eixo Y
     * @param low Lower Bound
     * @param high Upper Bound
     */
    void setVView(std::pair <double, double> v_view);
};

/**
 * @brief Plota o gráfico
 * @param func Função a ser plotada
 */
void plot(std::function<double(double)> func);

/**
 * @brief Declarações globais
 */
extern Settings settings;
extern std::function<double(double)> last_function;

#endif