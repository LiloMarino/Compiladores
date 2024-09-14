#ifndef UTILS_HPP
#define UTILS_HPP

#include <string>

// Controle de exceções
enum class ExceptionLevel
{
    WARNING,
    ERROR,
};

void throwException(ExceptionLevel level, bool lexical, int line, int column, const std::string &message);

void print(const std::string &token);

#endif
