#include "matrix.hpp"
#include "dcmat.hpp"
#include <iomanip>

Matrix::Matrix(size_t rows, size_t cols, double initialValue)
    : rows(rows), cols(cols), data(rows, std::vector<double>(cols, initialValue)) {}

size_t Matrix::getRows() const {
    return rows;
}

size_t Matrix::getCols() const {
    return cols;
}

double& Matrix::at(size_t row, size_t col) {
    return data.at(row).at(col);
}

const double& Matrix::at(size_t row, size_t col) const {
    return data.at(row).at(col);
}

void Matrix::printMatrix() const {
    // Imprimir o cabeçalho superior
    std::cout << "+-";
    for (size_t i = 0; i < cols * 10; ++i) {
        std::cout << " ";
    }
    std::cout << "-+\n";

    // Imprimir linhas da matriz
    std::cout << std::fixed << std::setprecision(6);
    for (const auto& row : data) {
        std::cout << "| ";
        for (const auto& value : row) {
            std::cout << std::setw(dcmat.settings.float_precision + 4) << value;
        }
        std::cout << " |\n";
    }

    // Imprimir o rodapé inferior
    std::cout << "+-";
    for (size_t i = 0; i < cols * 10; ++i) {
        std::cout << " ";
    }
    std::cout << "-+\n";
}
