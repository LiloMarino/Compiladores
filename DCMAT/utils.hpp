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
 * @brief Função de inicialização
 */
void init();

/**
 * @brief Lança uma mensagem de erro na saída padrão
 * @param level Nível da exceção
 * @param type Tipo da exceção
 * @param message Mensagem do erro
 */
void throwException(ExceptionLevel level, ExceptionType type, const std::string &message);

/**
 * @brief Função de erro do Bison
 * @param s Mensagem de erro do Bison
 */
void yyerror(const char *s);

#endif
