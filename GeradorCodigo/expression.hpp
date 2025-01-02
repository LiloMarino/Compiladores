#ifndef EXPRESSION_HPP
#define EXPRESSION_HPP

#include <string>
#include <memory>
#include <variant>
#include <optional>
#include <deque>

class Function;

enum class ExpressionType
{
    INTEGER,
    CHARACTER,
    STRING_LITERAL,
    IDENTIFIER,
    FUNCTION_CALL,
    ARRAY_ACCESS,
    UNARY,
    BINARY,
    TERNARY,
};

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
    NOT,
    TERNARY
};

class Expression
{
private:
    std::unique_ptr<Expression> condition = nullptr;                               // Condição para expressão ternária
    std::unique_ptr<Expression> left = nullptr;                                    // Subárvore esquerda
    std::unique_ptr<Expression> right = nullptr;                                   // Subárvore direita
    std::unique_ptr<std::deque<std::unique_ptr<Expression>>> parameters = nullptr; // Parâmetros para function call
    std::optional<std::variant<int, char, std::string>> value = std::nullopt;      // Folha
    OperatorType operatorSymbol;                                                   // Representação do operador
    ExpressionType type;                                                           // Tipo da expressão
public:
    /**
     * @brief Construtor para valores base (folhas)
     * @param val Valor da folha
     */
    Expression(const std::string &val, ExpressionType type);

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
     * @brief Construtor para operações ternárias
     * @param condition Condição da operação
     * @param true_expr Expressão verdadeira
     * @param false_expr Expressão falsa
     */
    Expression(std::unique_ptr<Expression> condition, std::unique_ptr<Expression> true_expr, std::unique_ptr<Expression> false_expr);

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
     * @brief Traduz a Expressão para código MIPS
     * @param reverse Indica se a condição da expressão deve ser avaliada invertida/negada
     * @param jumpTo Label que indica para onde deve saltar em caso da expressão ser avaliada como verdadeira
     * @param useRegister Indica qual registrador deve ser usado para resultado
     * @return O registrador da expressão
     */
    int translate(Function *func_context, bool reverse = false, const std::optional<std::string> &jumpTo = std::nullopt, int useRegister = -1);
private:

    /**
     * @brief Método auxiliar para lidar com variáveis globais
     * @param op Operação a ser feita
     * @param identifier Nome da variável global
     * @param r2 Registrador a ser operado
     */
    void handleGlobalVariable(OperatorType op, const std::string& identifier, int r2);
};

#endif