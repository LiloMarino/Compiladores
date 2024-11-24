#ifndef DYNAMIC_TYPING_HPP
#define DYNAMIC_TYPING_HPP

#include "matrix.hpp"
#include <iostream>
#include <stdexcept>

class DynamicTyping
{
private:
    union Data
    {
        double number;
        Matrix matrix;

        Data();
        ~Data();
    };

    enum class DataType
    {
        NONE,
        NUMBER,
        MATRIX
    };

    DataType type;
    Data data;

public:
    DynamicTyping();
    ~DynamicTyping();

    /**
     * @brief Método para limpar o membro ativo
     */
    void clear();

    /**
     * @brief Define o tipo como número
     * @param num O valor do número
     */
    void setNumber(double num);

    /**
     * @brief Define o tipo como matriz
     * @param m A matriz a ser armazenada
     */
    void setMatrix(Matrix &&m);

    /**
     * @brief Retorna se o tipo atual é um número
     * @return True se o tipo atual é um número, false caso contrário
     */
    bool isNumber() const;

    /**
     * @brief Retorna se o tipo atual é uma matriz
     * @return True se o tipo atual é uma matriz, false caso contrário
     */
    bool isMatrix() const;

    /**
     * @brief Obtém o número armazenado
     * @return O número armazenado
     */
    double getNumber() const;

    /**
     * @brief Obtém a matriz armazenada
     * @return A matriz armazenada
     */
    const Matrix &getMatrix() const;
};

#endif
