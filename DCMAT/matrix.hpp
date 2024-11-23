#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <vector>
#include <iostream>

class Matrix
{
private:
    size_t rows;                             // Número de linhas
    size_t cols;                             // Número de colunas
    std::vector<std::vector<double>> matrix; // Dados da matriz

public:
    /**
     * @brief Construtor
     * @param rows Linhas da matriz
     * @param cols Colunas da matriz
     * @param initialValue Valor inicial
     */
    Matrix(size_t rows, size_t cols, double initialValue = 0.0);

    /**
     * @brief Construtor
     * @param matrix Primeira linha da matriz
     */
    Matrix(std::vector<double> row);

    /**
     * @brief Construtor
     */
    Matrix();

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
     * @brief Inverte as linhas da matriz
     */
    void reverse();

    /**
     * @brief Calcula o determinante da matriz pelo método da eliminação de Gauss
     * @return Determinante da matriz
     * @throws std::invalid_argument Se a matriz não for quadrada
     */
    double determinant() const;

    /**
     * @brief Resolve o sistema linear
     * @return Vetor de soluções
     */
    std::vector<double> solveLinearSystem() const;

    /**
     * @brief Imprime a matriz formatada
     */
    void printMatrix() const;

    /**
     * @brief Soma 2 matrizes
     * @param other Outra matriz
     * @return Retorna a matriz resultante da soma
     */
    Matrix operator+(const Matrix &other) const;

    /**
     * @brief Concatena a linha a matriz
     * @param row Linha a adicionar
     * @return Retorna a matriz resultante da concatenação
     */
    Matrix operator+(const std::vector<double> &row) const;

    /**
     * @brief Soma 2 matrizes
     * @param other Outra matriz
     * @return Retorna a matriz resultante da soma
     */
    Matrix &operator+=(const Matrix &other);

    /**
     * @brief Concatena a linha a matriz
     * @param row Linha a adicionar
     * @return Retorna a matriz resultante da concatenação
     */
    Matrix &operator+=(const std::vector<double> &row);

private:
    /**
     * @brief Realiza a eliminação de gauss
     * @return Retorna a matriz triangular superior
     */
    std::vector<std::vector<double>> gaussianElimination(bool augmented = false) const;

};

#endif
