#include "dcmat.hpp"

// Declarações globais
Settings settings;
Function last_function;

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

void plot(std::function<double(double)> func)
{
    const int width = 80;
    const int height = 25;
    const double x_min = -10.0;
    const double x_max = 10.0;
    const double y_min = -10.0;
    const double y_max = 10.0;

    // Resolução do gráfico (25 linhas por 80 colunas)
    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            // Mapeia os valores x e y para os limites definidos
            double x_val = x_min + (x_max - x_min) * x / (width - 1);
            double y_val = func(x_val);

            // Normaliza a coordenada y para o intervalo de altura
            int y_pos = static_cast<int>((y_max - y_val) * (height - 1) / (y_max - y_min));

            if (y_pos == y)
            {
                std::cout << "*"; // Imprime o pixel
            }
            else
            {
                std::cout << " "; // Espaço vazio
            }
        }
        std::cout << std::endl;
    }
}

Function &Function::operator+=(std::function<double(double)> func)
{
    functions.push_back(func);
    return *this;
}

double Function::operator()(double x) const
{
    double result = x;
    for (const auto &func : functions)
    {
        result = func(result);
    }
    return result;
}

bool Function::empty() const
{
    return functions.empty();
}
