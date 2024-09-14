#include "utils.hpp"
#include <iostream>
#include <cstring>
#include <string>
#include <vector>

extern int yycolno;  // Coluna atual
extern int yylineno; // Linha atual
extern char *yytext;
bool firstLine = true;

std::vector<std::string> codeLines;

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
    yycolno += std::strlen(yytext);
}

void yyerror(const char *s)
{
    int line = yylineno;
    int column = yycolno - std::strlen(yytext);

    // Lançar exceção usando nossa função personalizada
    throwException(ExceptionLevel::ERROR, ExceptionType::SYNTAX, line, column, yytext);

    // Verifica se a linha existe no vector
    if (line > 0 && line <= codeLines.size())
    {
        // Exibe a linha de código onde ocorreu o erro
        std::string currentLine = codeLines[line - 1]; // Vetores são indexados a partir de 0
        std::cout << currentLine << std::endl;

        // Imprime espaços até a posição da coluna onde o erro ocorreu
        for (int i = 1; i < column; i++)
        {
            std::cout << " ";
        }

        // Imprime a seta "^" no local exato do erro
        std::cout << "^" << std::endl;
    }
}

void addLine(const std::string &line)
{
    codeLines.push_back(line);
}