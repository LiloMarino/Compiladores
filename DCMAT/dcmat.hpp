#ifndef DCMAT_HPP
#define DCMAT_HPP

#include <string>
#include <memory>
#include <unordered_map>
#include "function.hpp"
#include "matrix.hpp"
#include "dynamic_typing.hpp"

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

    /**
     * @brief Seta a precisão de exibição de números decimais
     * @param precision Número de casas decimais
     */
    void setFloatPrecision(int precision);
};

class DCMAT
{
public:
    Settings settings;

private:
    static const int WIDTH = 80;
    static const int HEIGHT = 25;
    char graph_matrix[HEIGHT][WIDTH];
    std::unordered_map<std::string, DynamicTyping> symbol_table;
    std::unique_ptr<Function> last_function = nullptr;
    std::unique_ptr<Matrix> last_matrix = nullptr;
    bool valid_expression = true;
    bool undefined_warning = true;
    std::string error_message;

public:
    /**
     * @brief Construtor
     */
    DCMAT();

    /**
     * @brief Configura se mostra ou não mensagens de warning de variáveis não definidas
     * @param enable Se true ativa o warning se false desativa o warning
     */
    void setUndefinedWarning(bool enable);

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

    /**
     * @brief Realiza o somatório
     * @param variable Variável do somatório
     * @param interval Intervalo da soma
     * @param f Função a ser somada
     * @return Valor da soma
     */
    double sum(const std::string &variable, std::pair<int, int> interval, const Function &f);

    /**
     * @brief Seta a variável
     * @param identifier Identificador da varíavel
     * @param var Valor da variável
     */
    void setVariable(const std::string &identifier, double value);

    /**
     * @brief Seta a variável
     * @param identifier Identificador da varíavel
     * @param matrix Matriz
     */
    void setVariable(const std::string &identifier, Matrix &&matrix);

    /**
     * @brief Seta a variável
     * @param identifier Identificador da varíavel
     * @param data Dados
     */
    void setVariable(const std::string &identifier, DynamicTyping &&data);

    /**
     * @brief Obtém a variável
     * @param identifier Identificador da varíavel
     * @return Valor da varíavel
     */
    DynamicTyping &getVariable(const std::string &identifier);

    /**
     * @brief Verifica a validez, se existem erros não tratados e reseta o estado de validez
     * @param reset Define se reseta ou não o estado de validez
     * @return Retorna true caso não existam erros, false caso existam erros
     */
    bool isValid(bool reset = true);

    /**
     * @brief Seta a última matriz
     * @param matrix Matriz
     */
    void setLastMatrix(std::unique_ptr<Matrix> matrix);

    /**
     * @brief Obtém a última matriz
     * @return Última matriz
     */
    Matrix &getLastMatrix() const;

    /**
     * @brief About
     */
    void about() const;

    /**
     * @brief Mostra os símbolos da tabela de símbolos
     */
    void showSymbols() const;

    /**
     * @brief Seta a mensagem de erro e marca a expressão com inválida
     * @param message Mensagem de erro
     */
    void setErrorMessage(const std::string &message);

    /**
     * @brief Obtém a mensagem de erro
     * @return Retorna a mensagem de erro
     */
    std::string getErrorMessage() const;

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