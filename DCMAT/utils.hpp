#ifndef UTILS_HPP
#define UTILS_HPP

#include <string>

// Controle de exceções
enum class ExceptionLevel
{
    WARNING,
    ERROR,
};

// Tipo do Erro
enum class ExceptionType
{
    NONE,
    LEXICAL,
    SYNTAX,
};

/**
 * @brief Lança uma mensagem de erro na saída padrão
 * @param level Nível da exceção
 * @param type Tipo da exceção
 * @param line Linha onde ocorreu
 * @param column Coluna onde ocorreu
 * @param message Mensagem do erro
 */
void throwException(ExceptionLevel level, ExceptionType type, int line, int column, const std::string &message);

/**
 * @brief Função de erro do Bison
 * @param s Mensagem de erro do Bison
 */
void yyerror(const char *s);

#endif
