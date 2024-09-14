#include "utils.hpp"
#include "sintatico.tab.h"
#include <iostream>
#include <cstring>
#include <string>
#include <vector>

extern int yychar;
extern int yycolno;  // Coluna atual
extern int yylineno; // Linha atual
extern std::string temp;
extern int yylex();
extern char *yytext;

std::vector<std::string> codeLines;
bool firstLine = true;
bool ignoreLexical = false;

void throwException(ExceptionLevel level, ExceptionType type, int line, int column, const std::string &message)
{
    if (type == ExceptionType::LEXICAL && ignoreLexical)
    {
        return;
    }
    
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

    if (level == ExceptionLevel::ERROR && type == ExceptionType::LEXICAL && !ignoreLexical)
    {
        exit(1);
    }
}

void yyerror(const char *s)
{
    // Pega a linha onde ocorreu o erro
    int line = yylineno;
    int column = yycolno - std::strlen(yytext);
    ignoreLexical = true;
    if (yychar == 0 || yychar == EOF_TOKEN)
    {
        throwException(ExceptionLevel::ERROR, ExceptionType::SYNTAX, line, column, "expected declaration or statement at end of input");
    }
    else
    {
        throwException(ExceptionLevel::ERROR, ExceptionType::SYNTAX, line, column, yytext);
    }

    // Lê toda a entrada
    while (yylex() != EOF_TOKEN)
    {
    }
    if (line > 0 && static_cast<size_t>(line) <= codeLines.size())
    {
        // Exibe a linha completa do código onde ocorreu o erro
        std::string currentLine = codeLines[line - 1];
        std::cout << std::endl
                  << currentLine << std::endl;

        // Imprime a seta
        for (int i = 1; i < column; i++)
        {
            std::cout << " ";
        }
        std::cout << "^";
    }
    else
    {
        // Exibe a linha completa do código onde ocorreu o erro
        std::cout << std::endl
                  << temp << std::endl;

        // Imprime a seta
        for (int i = 1; i < column; i++)
        {
            std::cout << " ";
        }
        std::cout << "^" << std::endl;
    }

    exit(1);
}

void addLine(const std::string &line)
{
    codeLines.push_back(line);
}