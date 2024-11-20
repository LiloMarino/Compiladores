#include "utils.hpp"
#include "sintatico.tab.h"
#include "dcmat.hpp"
#include <iostream>
#include <iomanip>
#include <string>

bool firstLine = true;

void init()
{
    std::cout << std::fixed << std::setprecision(dcmat.settings.float_precision);
}

void throwException(ExceptionLevel level, ExceptionType type, const std::string &message)
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
    std::cout << text << ": " << message;
    firstLine = false;

    if (level == ExceptionLevel::ERROR && type == ExceptionType::LEXICAL)
    {
        exit(1);
    }
}

void yyerror(const char *s)
{
    std::cout << s << std::endl;
}