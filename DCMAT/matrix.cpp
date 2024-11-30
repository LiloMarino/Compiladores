#include "matrix.hpp"
#include "dcmat.hpp"
#include <iomanip>
#include <algorithm>
#include <stdexcept>
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

double Matrix::determinant() const
{
    if (rows != cols)
    {
        throw std::invalid_argument("A matriz deve ser quadrada para calcular o determinante.");
    }

    // Executa a eliminação de Gauss
    std::vector<std::vector<double>> reducedMatrix = gaussianElimination();

    if (reducedMatrix.empty())
    {
        return 0.0;
    }

    // Calcula o determinante como o produto dos elementos da diagonal principal
    double det = 1.0;
    for (size_t i = 0; i < rows; ++i)
    {
        det *= reducedMatrix[i][i];
    }

    return det;
}

std::vector<double> Matrix::solveLinearSystem() const
{
    if (rows != cols - 1)
    {
        throw std::invalid_argument("A matriz não tem a forma adequada para um sistema linear (n x (n+1)).");
    }

    // Executa a eliminação de Gauss para a matriz aumentada
    std::vector<std::vector<double>> augmentedMatrix = gaussianElimination(true);

    // Verifica inconsistência (SI) ou infinitas soluções (SPI)
    size_t rank = 0;
    for (size_t i = 0; i < rows; ++i)
    {
        bool allZeros = true;
        for (size_t j = 0; j < cols - 1; ++j)
        {
            if (std::fabs(augmentedMatrix[i][j]) > 1e-9)
            { // Coeficientes diferentes de zero
                allZeros = false;
                break;
            }
        }

        if (allZeros)
        {
            if (std::fabs(augmentedMatrix[i][cols - 1]) > 1e-9)
            {
                throw std::runtime_error("SI - The Linear System has no solution");
            }
        }
        else
        {
            ++rank;
        }
    }

    if (rank < cols - 1)
    {
        throw std::runtime_error("SPI - The Linear System has infinitely many solutions");
    }

    // Resolver o sistema linear por substituição reversa
    size_t n = rows;
    std::vector<double> solution(n);
    for (int i = n - 1; i >= 0; --i)
    {
        solution[i] = augmentedMatrix[i][n];
        for (size_t j = i + 1; j < n; ++j)
        {
            solution[i] -= augmentedMatrix[i][j] * solution[j];
        }
        solution[i] /= augmentedMatrix[i][i];
    }

    return solution;
}

void Matrix::printMatrix() const
{
    const int padding = 1;

    // Determina a largura máxima com base no maior número
    int max_width = 0;
    for (const auto &row : matrix)
    {
        for (const auto &value : row)
        {
            std::ostringstream oss;
            oss << std::fixed << std::setprecision(dcmat.settings.float_precision) << value;
            int width = oss.str().length(); // Obtém a largura da string com o número formatado
            max_width = std::max(max_width, width);
        }
    }
    max_width += padding;

    // Imprimir o cabeçalho superior
    std::cout << "+-" << std::setw(max_width * cols + padding) << "-+" << std::endl;

    // Imprimir linhas da matriz
    std::cout << std::fixed << std::setprecision(dcmat.settings.float_precision);
    for (const auto &row : matrix)
    {
        std::cout << "|";
        for (const auto &value : row)
        {
            std::cout << std::setw(max_width) << value;
        }
        std::cout << std::setw(padding + 1) << "|" << std::endl;
    }

    // Imprimir o rodapé inferior
    std::cout << "+-" << std::setw(max_width * cols + padding) << "-+" << std::endl;
}

Matrix Matrix::operator+(const Matrix &other) const
{
    Matrix result = *this;
    result += other;
    return result;
}

Matrix Matrix::operator-(const Matrix &other) const
{
    Matrix result = *this;
    result -= other;
    return result;
}

Matrix Matrix::operator*(const Matrix &other) const
{
    Matrix result = *this;
    result *= other;
    return result;
}

Matrix Matrix::operator+(const std::vector<double> &row) const
{
    Matrix result = *this;
    result += row;
    return result;
}

Matrix Matrix::operator*(int scalar) const
{
    Matrix result = *this;
    result *= scalar;
    return result;
}

Matrix Matrix::operator*(double scalar) const
{
    Matrix result = *this;
    result *= scalar;
    return result;
}

Matrix &Matrix::operator+=(const Matrix &other)
{
    if (rows != other.rows || cols != other.cols)
    {
        dcmat.setErrorMessage("Incorrect dimensions for operator '+' - have MATRIX [" +
                              std::to_string(rows) + "][" + std::to_string(cols) + "] and MATRIX [" +
                              std::to_string(other.rows) + "][" + std::to_string(other.cols) + "]");
        return *this;
    }
    for (size_t i = 0; i < rows; ++i)
    {
        for (size_t j = 0; j < cols; ++j)
        {
            matrix[i][j] += other.matrix[i][j];
        }
    }
    return *this;
}

Matrix &Matrix::operator-=(const Matrix &other)
{
    if (rows != other.rows || cols != other.cols)
    {
        dcmat.setErrorMessage("Incorrect dimensions for operator '-' - have MATRIX [" +
                              std::to_string(rows) + "][" + std::to_string(cols) + "] and MATRIX [" +
                              std::to_string(other.rows) + "][" + std::to_string(other.cols) + "]");
        return *this;
    }
    for (size_t i = 0; i < rows; ++i)
    {
        for (size_t j = 0; j < cols; ++j)
        {
            matrix[i][j] -= other.matrix[i][j];
        }
    }
    return *this;
}

Matrix &Matrix::operator*=(const Matrix &other)
{
    if (cols != other.rows)
    {
        dcmat.setErrorMessage("Incorrect dimensions for operator '*' - have MATRIX [" +
                              std::to_string(rows) + "][" + std::to_string(cols) + "] and MATRIX [" +
                              std::to_string(other.rows) + "][" + std::to_string(other.cols) + "]");
        return *this;
    }
    Matrix result(rows, other.cols, 0.0);
    for (size_t i = 0; i < rows; ++i)
    {
        for (size_t j = 0; j < other.cols; ++j)
        {
            for (size_t k = 0; k < cols; ++k)
            {
                result.matrix[i][j] += matrix[i][k] * other.matrix[k][j];
            }
        }
    }
    *this = std::move(result);
    return *this;
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

Matrix &Matrix::operator*=(int scalar)
{
    for (size_t i = 0; i < rows; ++i)
    {
        for (size_t j = 0; j < cols; ++j)
        {
            matrix[i][j] *= scalar;
        }
    }
    return *this;
}

Matrix &Matrix::operator*=(double scalar)
{
    for (size_t i = 0; i < rows; ++i)
    {
        for (size_t j = 0; j < cols; ++j)
        {
            matrix[i][j] *= scalar;
        }
    }
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