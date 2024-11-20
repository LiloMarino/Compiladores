#include "dcmat.hpp"

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

Function::Function(std::function<double(double)> op)
    : unaryOperation(op), left(nullptr), right(nullptr) {}

Function::Function(std::function<double(double)> op, std::unique_ptr<Function> child)
    : unaryOperation(op), left(std::move(child)), right(nullptr) {}

Function::Function(std::function<double(double, double)> op, std::unique_ptr<Function> l, std::unique_ptr<Function> r)
    : binaryOperation(op), left(std::move(l)), right(std::move(r)) {}

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
            double y_val = (*last_function)(x_val);

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
