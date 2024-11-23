#include "matrix.hpp"
#include "dcmat.hpp"
#include <iomanip>
#include <algorithm>

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
