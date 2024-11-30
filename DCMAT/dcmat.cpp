#include "dcmat.hpp"
#include <iostream>
#include <iomanip>

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

void Settings::setFloatPrecision(int precision)
{
    if (precision >= 0 && precision <= 8)
    {
        float_precision = precision;
        std::cout << std::fixed << std::setprecision(float_precision);
    }
    else
    {
        std::cout << "ERROR: float precision must be from 0 to 8" << std::endl;
    }
}

DCMAT::DCMAT()
{
    clearGraph();
}

void DCMAT::setUndefinedWarning(bool enable)
{
    undefined_warning = enable;
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
    const double x_step = (x_max - x_min) / (WIDTH - 1);

    if (settings.erase_plot)
    {
        clearGraph();
    }
    if (settings.draw_axis)
    {
        drawAxis();
    }

    for (int col = 0; col < WIDTH; ++col)
    {
        double x = x_min + x_step * col;
        double y = (*last_function)(x);

        // Verificar se y está dentro dos limites y_min e y_max
        if (y < y_min)
        {
            y = y_min;
        }
        else if (y > y_max)
        {
            y = y_max;
        }

        // Mapeia o valor de y para a coordenada da matriz de altura
        int row = static_cast<int>((y_max - y) * (HEIGHT - 1) / (y_max - y_min));

        // Corrige para bordas da matriz
        if (row < 0)
            row = 0;
        if (row >= HEIGHT)
            row = HEIGHT - 1;

        graph_matrix[row][col] = '*';
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
    for (double var = getVariable(variable).getNumber(); var <= interval.second; setVariable(variable, ++var))
    {
        sum += f(var);
    }
    return sum;
}

void DCMAT::setVariable(const std::string &identifier, double value)
{
    symbol_table[identifier] = value;
}

void DCMAT::setVariable(const std::string &identifier, Matrix &&matrix)
{
    symbol_table[identifier] = std::move(matrix);
}

void DCMAT::setVariable(const std::string &identifier, DynamicTyping &&data)
{
    symbol_table[identifier] = std::move(data);
}

DynamicTyping &DCMAT::getVariable(const std::string &identifier)
{
    auto it = symbol_table.find(identifier);
    if (it != symbol_table.end())
    {
        return it->second;
    }
    else
    {
        valid_expression = false;
        if (undefined_warning)
        {
            std::cout << "Undefined symbol [" << identifier << "]" << std::endl;
        }
        // Objeto estático para retornar uma referência válida
        static DynamicTyping dummy;
        return dummy;
    }
}

bool DCMAT::isValidExpression(bool discard)
{
    bool tmp = valid_expression;
    if (discard)
    {
        valid_expression = true;
    }
    return tmp;
}

void DCMAT::setLastMatrix(std::unique_ptr<Matrix> matrix)
{
    last_matrix = std::move(matrix);
}

Matrix &DCMAT::getLastMatrix() const
{
    if (!last_matrix)
    {
        throw std::runtime_error("No Matrix defined!");
    }
    return *last_matrix;
}

void DCMAT::about() const
{
    const std::string title("202200560390 - Murilo Aldigueri Marino");
    const std::string version("V. 2024.01");

    // Calcula a largura mínima com base no maior texto (título ou versão)
    const int padding = 4;
    const int width = std::max(static_cast<int>(title.length()), static_cast<int>(version.length())) + padding;
    const std::string border(width, '-'); // Linha superior e inferior do banner

    // Exibição do banner com alinhamento
    std::cout << "+" << border << "+" << std::endl;
    std::cout << "|" << std::setw(width) << std::setfill(' ') << " " << "|" << std::endl;
    std::cout << "|" << std::setw((width + title.length()) / 2)
              << title
              << std::setw((width - title.length()) / 2) << " " << "|" << std::endl;
    std::cout << "|" << std::setw((width + version.length()) / 2)
              << version
              << std::setw((width - version.length()) / 2) << " " << "|" << std::endl;
    std::cout << "|" << std::setw(width) << std::setfill(' ') << " " << "|" << std::endl;
    std::cout << "+" << border << "+" << std::endl;
}

void DCMAT::showSymbols() const
{
    std::cout << std::endl;
    for (const auto &[key, value] : symbol_table)
    {
        std::string text = value.isNumber() ? "FLOAT" : "MATRIX";
        if (value.isMatrix())
        {
            text += " [";
            text += std::to_string(value.getMatrix().getRows());
            text += "][";
            text += std::to_string(value.getMatrix().getCols());
            text += "]";
        }
        std::cout << key << " - " << text << std::endl;
    }
    std::cout << std::endl;
}

void DCMAT::setErrorMessage(const std::string &message)
{
    valid_expression = false;
    error_message = message;
}

std::string DCMAT::getErrorMessage() const
{
    return error_message;
}

void DCMAT::drawAxis()
{
    const double x_min = settings.h_view_lo;
    const double x_max = settings.h_view_hi;
    const double y_min = settings.v_view_lo;
    const double y_max = settings.v_view_hi;

    // Cálculo da posição do eixo X (horizontal)
    int y_axis_pos = (y_max >= 0 && y_min <= 0)
                         ? static_cast<int>((y_max) * (HEIGHT - 1) / (y_max - y_min))
                         : (y_min > 0 ? HEIGHT - 1 : 0);

    // Cálculo da posição do eixo Y (vertical)
    int x_axis_pos = (x_max > 0 && x_min < 0)
                         ? static_cast<int>((-x_min) * (WIDTH - 1) / (x_max - x_min))
                         : (x_min >= 0 ? 0 : WIDTH - 1);

    // Desenhando o eixo X
    if (y_axis_pos >= 0 && y_axis_pos < HEIGHT)
    {
        for (int x = 0; x < WIDTH; ++x)
        {
            graph_matrix[y_axis_pos][x] = '-';
        }
    }

    // Desenhando o eixo Y
    if (x_axis_pos >= 0 && x_axis_pos < WIDTH)
    {
        for (int y = 0; y < HEIGHT; ++y)
        {
            graph_matrix[y][x_axis_pos] = '|';
        }
    }

    // Marcar a origem se visível
    if (y_axis_pos >= 0 && y_axis_pos < HEIGHT && x_axis_pos >= 0 && x_axis_pos < WIDTH)
    {
        graph_matrix[y_axis_pos][x_axis_pos] = '+';
    }
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
