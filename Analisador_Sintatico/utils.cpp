#include "utils.hpp"
#include <iostream>
#include <cstring>
#include <string>
#include <cstdio>

extern int yycolno;  // Coluna atual
extern int yylineno; // Linha atual
extern char *yytext;
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
    yycolno += std::strlen(yytext);
}

// Função que exibe a linha de código com a posição do erro
void yyerror(const char *s)
{
    int line = yylineno;
    int column = yycolno - std::strlen(yytext) + 1;

    // Lançar exceção usando nossa função personalizada
    throwException(ExceptionLevel::ERROR, ExceptionType::SYNTAX, line, column, yytext);

    // Exibe a linha onde ocorreu o erro
    std::string currentLine = getLine(yylineno);
    std::cout << currentLine << std::endl;

    // Imprime espaços até a posição da yycolno onde o erro ocorreu
    for (int i = 1; i < column; i++)
    {
        std::cout << " ";
    }
    // Imprime a seta "^" no local exato do erro
    std::cout << "^" << std::endl;
}

std::string getLine(int nline)
{
    std::string line;

    // Avança até a linha desejada
    for (int i = 0; i < nline; i++)
    {
        std::string tempLine;
        if (std::getline(std::cin, tempLine))
        {
            // Lê a linha e armazena na linha de buffer
            line = std::move(tempLine);
        }
        else
        {
            // Se não houver mais linhas, quebra o loop
            break;
        }
    }

    // Remove caracteres indesejados, se existirem
    if (!line.empty() && line[0] == '\n')
    {
        line[0] = ' ';
    }
    if (!line.empty() && line.back() == '\n')
    {
        line.pop_back();
    }

    return line;
}
