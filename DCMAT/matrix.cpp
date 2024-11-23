#include "matrix.hpp"
#include "dcmat.hpp"
#include <iomanip>
#include <algorithm>
#include <cmath>

Matrix::Matrix(size_t rows, size_t cols, double initialValue)
    : rows(rows), cols(cols), matrix(rows, std::vector<double>(cols, initialValue)) {}

Matrix::Matrix(std::vector<double> row)
    : rows(1), cols(matrix.size()), matrix({row}) {}

Matrix::Matrix()
    : rows(0), cols(0), matrix() {}

size_t Matrix::getRows() const
{
    return rows;
}

size_t Matrix::getCols() const
{
    return cols;
}

double &Matrix::at(size_t row, size_t col)
{
    return matrix.at(row).at(col);
}

const double &Matrix::at(size_t row, size_t col) const
{
    return matrix.at(row).at(col);
}

void Matrix::reverse()
{
    std::reverse(matrix.begin(), matrix.end());
}

double Matrix::determinant() const {
    if (rows != cols) {
        throw std::invalid_argument("A matriz deve ser quadrada para calcular o determinante");
    }

    size_t n = rows;
    std::vector<std::vector<double>> tempMatrix = matrix; // Copia a matriz original
    double det = 1.0;                                     // Inicializa o determinante
    int swaps = 0;                                        // Contador de trocas de linhas

    for (size_t col = 0; col < n; ++col) {
        // Encontrar o pivot (elemento não zero mais próximo do topo)
        size_t pivotRow = col;
        for (size_t row = col + 1; row < n; ++row) {
            if (std::fabs(tempMatrix[row][col]) > std::fabs(tempMatrix[pivotRow][col])) {
                pivotRow = row;
            }
        }

        // Se o pivot for zero, o determinante é zero
        if (std::fabs(tempMatrix[pivotRow][col]) < 1e-9) {
            return 0.0;
        }

        // Trocar linhas, se necessário
        if (pivotRow != col) {
            std::swap(tempMatrix[col], tempMatrix[pivotRow]);
            swaps++;
        }

        // Eliminar elementos abaixo do pivot
        for (size_t row = col + 1; row < n; ++row) {
            double factor = tempMatrix[row][col] / tempMatrix[col][col];
            for (size_t k = col; k < n; ++k) {
                tempMatrix[row][k] -= factor * tempMatrix[col][k];
            }
        }

        // Multiplicar pelo elemento da diagonal principal
        det *= tempMatrix[col][col];
    }

    // Ajustar o determinante com base no número de trocas de linhas
    if (swaps % 2 != 0) {
        det = -det;
    }

    return det;
}

void Matrix::printMatrix() const
{
    // Imprimir o cabeçalho superior
    std::cout << "+-";
    for (size_t i = 0; i < cols * 10; ++i)
    {
        std::cout << " ";
    }
    std::cout << "-+\n";

    // Imprimir linhas da matriz
    std::cout << std::fixed << std::setprecision(6);
    for (const auto &row : matrix)
    {
        std::cout << "| ";
        for (const auto &value : row)
        {
            std::cout << std::setw(dcmat.settings.float_precision + 4) << value;
        }
        std::cout << " |\n";
    }

    // Imprimir o rodapé inferior
    std::cout << "+-";
    for (size_t i = 0; i < cols * 10; ++i)
    {
        std::cout << " ";
    }
    std::cout << "-+\n";
}

Matrix Matrix::operator+(const std::vector<double> &row) const
{
    Matrix result = *this;
    result += row;
    return result;
}

Matrix &Matrix::operator+=(const std::vector<double> &row)
{
    if (row.size() < cols)
    {
        // Caso o vetor seja menor, preenche com zeros
        std::vector<double> expandedRow = row;
        expandedRow.resize(cols, 0.0);
        matrix.push_back(expandedRow);
    }
    else if (row.size() > cols)
    {
        // Caso o vetor seja maior, expande todas as linhas da matriz
        size_t newCols = row.size();
        for (auto &existingRow : matrix)
        {
            existingRow.resize(newCols, 0.0);
        }
        matrix.push_back(row);
        cols = newCols;
    }
    else
    {
        // Caso os tamanhos sejam iguais, adiciona diretamente
        matrix.push_back(row);
    }

    rows += 1;
    return *this;
}
