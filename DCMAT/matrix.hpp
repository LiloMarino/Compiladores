#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <vector>
#include <iostream>

class Matrix
{
private:
    size_t rows;                           // Número de linhas
    size_t cols;                           // Número de colunas
    std::vector<std::vector<double>> data; // Dados da matriz

public:
    /**
     * @brief Construtor
     * @param rows Linhas da matriz
     * @param cols Colunas da matriz
     * @param initialValue Valor inicial
     */
    Matrix(size_t rows, size_t cols, double initialValue = 0.0);

    /**
     * @brief Obtém o número de linhas
     * @return Número de linhas
     */
    size_t getRows() const;

    /**
     * @brief Obtém o número de colunas
     * @return Número de colunas
     */
    size_t getCols() const;

    /**
     * @brief Acessa a matriz (read-write)
     * @param row Linha
     * @param col Coluna
     * @return Valor M[row][col]
     */
    double &at(size_t row, size_t col);

    /**
     * @brief Acessa a matriz (read-only)
     * @param row Linha
     * @param col Coluna
     * @return Valor M[row][col]
     */
    const double &at(size_t row, size_t col) const;

    /**
     * @brief Imprime a matriz formatada
     */
    void printMatrix() const;
};

#endif
