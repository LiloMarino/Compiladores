#ifndef VARIABLE_HPP
#define VARIABLE_HPP

#include <string>
#include "type.hpp"

// Declaração antecipada para evitar dependência circular
class Function;

enum class VariableCategory
{
    CONSTANT,
    PARAMETER,
    GLOBAL_VARIABLE,
    LOCAL_VARIABLE,
};

class Variable
{
private:
    std::unique_ptr<Type> type = nullptr;
    VariableCategory category;
    std::string identifier;
    int value = 0;

public:
    /**
     * @brief Construtor de variável
     * @param category Categoria da variável
     * @param type Tipo da variável
     * @param identifier Identificador da variável
     */
    Variable(VariableCategory category, std::unique_ptr<Type> type, const std::string &identifier);

    /**
     * @brief Construtor de constante
     * @param value Valor da constante
     * @param identifier Identificador da constante
     */
    Variable(const std::string &identifier, const int value);

    /**
     * @brief Obtém o identificador
     * @return Identificador da variável
     */
    std::string getIdentifier() const;

    /**
     * @brief Obtém o tipo da variável
     * @return Tipo da variável
     */
    const Type &getType() const;

    /**
     * @brief Traduz a Variável para código MIPS
     * @param func_context Contexto da função
     */
    void translate(Function *func_context = nullptr);
};

#endif