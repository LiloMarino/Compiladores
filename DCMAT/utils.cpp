#include "utils.hpp"
#include "sintatico.tab.h"
#include <iostream>
#include <cstring>
#include <string>
#include <vector>

bool firstLine = true;

void throwException(ExceptionLevel level, ExceptionType type, int line, int column, const std::string &message)
{
    std::string text;
    switch (level)
    {
    case ExceptionLevel::ERROR:
        text += "error:";
        break;
    case ExceptionLevel::WARNING:
        text += "warning:";
        break;
    default:
        break;
    }
    switch (type)
    {
    case ExceptionType::LEXICAL:
        text += "lexical:";
        break;
    case ExceptionType::SYNTAX:
        text += "syntax:";
        break;
    default:
        break;
    }

    if (!firstLine)
    {
        std::cout << "\n";
    }
    std::cout << text << line << ":" << column << ": " << message;
    firstLine = false;

    if (level == ExceptionLevel::ERROR && type == ExceptionType::LEXICAL)
    {
        exit(1);
    }
}

void yyerror(const char *s) {
    std::cout << s << std::endl;
}