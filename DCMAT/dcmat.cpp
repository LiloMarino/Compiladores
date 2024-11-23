#include "dcmat.hpp"
#include <iostream>


DCMAT dcmat;

void Settings::show() const
{
    std::cout << std::endl
              << "h_view_lo: " << h_view_lo << std::endl
              << "h_view_hi: " << h_view_hi << std::endl
              << "v_view_lo: " << v_view_lo << std::endl
              << "v_view_hi: " << v_view_hi << std::endl
              << "float precision: " << float_precision << std::endl
              << "integral_steps: " << integral_steps << std::endl
              << std::endl
              << "Draw Axis: " << (draw_axis ? "ON" : "OFF") << std::endl
              << "Erase Plot: " << (erase_plot ? "ON" : "OFF") << std::endl
              << "Connect Dots: " << (connect_dots ? "ON" : "OFF") << std::endl
              << std::endl;
}

void Settings::reset()
{
    h_view_lo = -6.5;
    h_view_hi = 6.5;
    v_view_lo = -3.5;
    v_view_hi = 3.5;
    float_precision = 6;
    integral_steps = 1000;
    draw_axis = true;
    erase_plot = true;
    connect_dots = false;
}

void Settings::setHView(std::pair<double, double> h_view)
{
    h_view_lo = h_view.first;
    h_view_hi = h_view.second;
}

void Settings::setVView(std::pair<double, double> v_view)
{
    v_view_lo = v_view.first;
    v_view_hi = v_view.second;
}

DCMAT::DCMAT()
{
    clearGraph();
}

void DCMAT::setLastFunction(std::unique_ptr<Function> func)
{
    last_function = std::move(func);
}

void DCMAT::plot()
{
    if (!last_function)
    {
        std::cout << "No function defined!" << std::endl;
        return;
    }

    const double x_min = settings.h_view_lo;
    const double x_max = settings.h_view_hi;
    const double y_min = settings.v_view_lo;
    const double y_max = settings.v_view_hi;

    if (settings.erase_plot)
    {
        clearGraph();
    }
    if (settings.draw_axis)
    {
        drawAxis();
    }

    for (int y = 0; y < HEIGHT; ++y)
    {
        for (int x = 0; x < WIDTH; ++x)
        {
            // Mapeia os valores de x e y para os limites definidos
            double x_val = x_min + (x_max - x_min) * x / (WIDTH - 1);
            double y_val = (*last_function)(x_val);

            // Normaliza a coordenada y para o intervalo de altura
            int y_pos = static_cast<int>((y_max - y_val) * (HEIGHT - 1) / (y_max - y_min));

            // Se a posição y na matriz coincidir com o valor calculado, desenha o ponto
            if (y_pos == y)
            {
                graph_matrix[y][x] = '*';
            }
        }
    }

    renderGraph();
}

double DCMAT::integrate(std::pair<double, double> interval, const Function &f)
{
    const double step = (interval.second - interval.first) / settings.integral_steps;
    double sum = 0.0;

    for (double x = interval.first; x < interval.second; x += step)
    {
        sum += f(x) * step;
    }

    return sum;
}

double DCMAT::sum(const std::string &variable, std::pair<int, int> interval, const Function &f)
{
    double sum = 0.0;
    setVariable(variable, interval.first);
    for (double var = getVariable(variable); var <= interval.second; setVariable(variable, ++var))
    {
        sum += f(var);
    }
    return sum;
}

void DCMAT::setVariable(const std::string &identifier, double value)
{
    symbol_table[identifier] = value;
}

double DCMAT::getVariable(const std::string &identifier)
{
    double value = symbol_table.at(identifier);
    return value;
}

void DCMAT::setLastMatrix(std::unique_ptr<Matrix> matrix)
{
    last_matrix = std::move(matrix);
}

Matrix &DCMAT::getLastMatrix() const {
    if (!last_matrix) {
        throw std::runtime_error("No Matrix defined!");
    }
    return *last_matrix;
}

void DCMAT::drawAxis()
{
    // Definindo a posição do eixo Y (vertical)
    int y_axis_pos = HEIGHT / 2;

    // Desenhando o eixo Y
    for (int x = 0; x < WIDTH; ++x)
    {
        graph_matrix[y_axis_pos][x] = '-'; // Eixo X
    }

    // Definindo a posição do eixo X (horizontal)
    int x_axis_pos = WIDTH / 2;

    // Desenhando o eixo X
    for (int y = 0; y < HEIGHT; ++y)
    {
        graph_matrix[y][x_axis_pos] = '|'; // Eixo Y
    }

    // Centralizando a origem (0,0)
    graph_matrix[y_axis_pos][x_axis_pos] = '+';
}

void DCMAT::clearGraph()
{
    // Preenche a matriz com espaços vazios
    for (int y = 0; y < HEIGHT; ++y)
    {
        for (int x = 0; x < WIDTH; ++x)
        {
            graph_matrix[y][x] = ' ';
        }
    }
}

void DCMAT::renderGraph() const
{
    // Imprime a matriz de gráfico no terminal
    for (int y = 0; y < HEIGHT; ++y)
    {
        for (int x = 0; x < WIDTH; ++x)
        {
            std::cout << graph_matrix[y][x];
        }
        std::cout << std::endl;
    }
}
