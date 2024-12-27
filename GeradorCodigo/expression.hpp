#ifndef EXPRESSION_HPP
#define EXPRESSION_HPP

#include <string>
#include <memory>
#include <variant>
#include <optional>
#include <functional>
#include <deque>

enum class OperatorType
{
    NONE,
    PLUS,
    MINUS,
    MULTIPLY,
    DIVIDE,
    REMAINDER,
    BITWISE_AND,
    BITWISE_OR,
    BITWISE_XOR,
    LOGICAL_AND,
    LOGICAL_OR,
    EQUAL,
    NOT_EQUAL,
    LESS_THAN,
    GREATER_THAN,
    LESS_EQUAL,
    GREATER_EQUAL,
    R_SHIFT,
    L_SHIFT,
    ASSIGN,
    ADD_ASSIGN,
    MINUS_ASSIGN,
    INC,
    DEC,
    BITWISE_NOT,
    NOT
};

class Expression
{
private:
    std::unique_ptr<Expression> left;                                      // Subárvore esquerda
    std::unique_ptr<Expression> right;                                     // Subárvore direita
    std::unique_ptr<std::deque<std::unique_ptr<Expression>>> parameters;   // Parâmetros para function call
    std::optional<std::variant<int, char, std::string>> value;             // Folha
    OperatorType operatorSymbol;                                           // Representação do operador
public:
    /**
     * @brief Construtor para valores base (folhas)
     * @param val Valor da folha
     */
    Expression(const std::string &val);

    /**
     * @brief Construtor para valores base (folhas)
     * @param val Valor da folha
     */
    Expression(const char val);
    
    /**
     * @brief Construtor para valores base (folhas)
     * @param val Valor da folha
     */
    Expression(const int val);

    /**
     * @brief Construtor para chamadas de função
     * @param identifier Identificador da chamada
     * @param parameters Parâmetros da chamada
     */
    Expression(const std::string &identifier, std::unique_ptr<std::deque<std::unique_ptr<Expression>>> parameters);

    /**
     * @brief Construtor para acesso índices de array
     * @param identifier Identificador do array
     * @param index Expressão para o índice do array
     */
    Expression(const std::string &identifier, std::unique_ptr<Expression> index);

    /**
     * @brief Construtor para operações unárias
     * @param child Subárvore
     * @param symbol Símbolo do operador
     */
    Expression(std::unique_ptr<Expression> child, const OperatorType symbol);

    /**
     * @brief Construtor para operações binárias
     * @param left Subárvore à esquerda
     * @param right Subárvore à direita
     * @param symbol Símbolo do operador
     */
    Expression(std::unique_ptr<Expression> left, std::unique_ptr<Expression> right, const OperatorType symbol);

    /**
     * @brief Obtém o operador associado ao nó, caso ele seja um nó operador.
     *
     * @return OperatorType O operador associado ao nó.
     * @throws std::logic_error Se o nó não for um nó operador.
     */
    OperatorType getOperator() const;

    /**
     * @brief Obtém o valor armazenado no nó, caso ele seja uma folha.
     *
     * @return std::optional<std::variant<int, char, std::string>> O valor armazenado no nó, ou std::nullopt se o nó não for uma folha.
     */
    std::optional<std::variant<int, char, std::string>> getValue() const;

    /**
     * @brief Verifica se o nó é uma folha (possui um valor).
     *
     * @return true Se o nó é uma folha.
     * @return false Se o nó não é uma folha.
     */
    bool isValueNode() const;

    /**
     * @brief Verifica se o nó é um operador (possui um símbolo de operador válido).
     *
     * @return true Se o nó é um operador.
     * @return false Se o nó não é um operador.
     */
    bool isOperatorNode() const;

    /**
     * @brief Aplica uma função ao nó atual da expressão e recursivamente aos filhos em pré-ordem.
     *
     * A função é aplicada ao nó atual, seguida das subárvores esquerda e direita, se existirem.
     *
     * @param func Função que recebe uma referência constante a um objeto `Expression` e será aplicada ao nó atual.
     */
    void applyPreOrder(const std::function<void(const Expression &)> &func) const;
};

#endif