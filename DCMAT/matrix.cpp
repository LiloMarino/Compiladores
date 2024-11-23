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
        throw std::invalid_argument("A matriz deve ser quadrada para calcular o determinante.");
    }

    // Executa a eliminação de Gauss
    std::vector<std::vector<double>> reducedMatrix = gaussianElimination();

    if (reducedMatrix.empty()) {
        return 0.0;
    }

    // Calcula o determinante como o produto dos elementos da diagonal principal
    double det = 1.0;
    for (size_t i = 0; i < rows; ++i) {
        det *= reducedMatrix[i][i];
    }

    return det;
}

std::vector<double> Matrix::solveLinearSystem() const {
    if (rows != cols - 1) {
        throw std::invalid_argument("A matriz não tem a forma adequada para um sistema linear (n x (n+1)).");
    }

    // Executa a eliminação de Gauss para a matriz aumentada
    std::vector<std::vector<double>> augmentedMatrix = gaussianElimination(true);

    // Verifica inconsistência (SI) ou infinitas soluções (SPI)
    size_t rank = 0;
    for (size_t i = 0; i < rows; ++i) {
        bool allZeros = true;
        for (size_t j = 0; j < cols - 1; ++j) {
            if (std::fabs(augmentedMatrix[i][j]) > 1e-9) { // Coeficientes diferentes de zero
                allZeros = false;
                break;
            }
        }

        if (allZeros) {
            if (std::fabs(augmentedMatrix[i][cols - 1]) > 1e-9) {
                throw std::runtime_error("SI - The Linear System has no solution");
            }
        } else {
            ++rank;
        }
    }

    if (rank < cols - 1) {
        throw std::runtime_error("SPI - The Linear System has infinitely many solutions");
    }

    // Resolver o sistema linear por substituição reversa
    size_t n = rows;
    std::vector<double> solution(n);
    for (int i = n - 1; i >= 0; --i) {
        solution[i] = augmentedMatrix[i][n];
        for (size_t j = i + 1; j < n; ++j) {
            solution[i] -= augmentedMatrix[i][j] * solution[j];
        }
        solution[i] /= augmentedMatrix[i][i];
    }

    return solution;
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

std::vector<std::vector<double>> Matrix::gaussianElimination(bool augmented) const
{
    size_t n = rows;
    size_t m = augmented ? cols : rows;                   // Se for matriz aumentada, considerar todas as colunas
    std::vector<std::vector<double>> tempMatrix = matrix; // Copia a matriz original

    // Realiza a eliminação de Gauss
    for (size_t col = 0; col < n; ++col)
    {
        // Encontrar o pivot (elemento não zero mais próximo do topo)
        size_t pivotRow = col;
        for (size_t row = col + 1; row < n; ++row)
        {
            if (std::fabs(tempMatrix[row][col]) > std::fabs(tempMatrix[pivotRow][col]))
            {
                pivotRow = row;
            }
        }

        // Trocar linhas, se necessário
        if (pivotRow != col)
        {
            std::swap(tempMatrix[col], tempMatrix[pivotRow]);
        }

        // Eliminar elementos abaixo do pivot
        for (size_t row = col + 1; row < n; ++row)
        {
            double factor = tempMatrix[row][col] / tempMatrix[col][col];
            for (size_t k = col; k < m; ++k)
            { 
                // Considera todas as colunas (matriz aumentada ou quadrada)
                tempMatrix[row][k] -= factor * tempMatrix[col][k];
            }
        }
    }

    return tempMatrix;
}