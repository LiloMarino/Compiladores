#include "utils.hpp"
#include <iostream>
#include <cstring>

extern int coluna;
extern char *yytext;
bool firstLine = true;

void throwException(ExceptionLevel level, bool lexical, int line, int column, const std::string &message)
{
    std::string text;
    if (level == ExceptionLevel::WARNING)
    {
        text += "warning:";
    }
    else if (level == ExceptionLevel::ERROR)
    {
        text += "error:";
    }
    if (lexical)
    {
        text += "lexical:";
    }

    if (!firstLine)
    {
        std::cout << "\n";
    }
    std::cout << text << line << ":" << column << ": " << message;
    firstLine = false;
    coluna += std::strlen(yytext);
}

void print(const std::string &token)
{
    if (firstLine)
    {
        std::cout << token;
        firstLine = false;
    }
    else
    {
        std::cout << "\n"
                  << token;
    }
    coluna += std::strlen(yytext);
}