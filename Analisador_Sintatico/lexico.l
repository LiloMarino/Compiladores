%option noyywrap
%option yylineno
%x COMMENT
%{
#include <iostream>
#include <string>
#include <cstring>
#include "utils.hpp"

// Variáveis globais
int coluna = 1;
int inicioLinha;
int inicioColuna;
std::string result;

%}
%%

"/*" { 
    BEGIN(COMMENT); 
    inicioLinha = yylineno;
    inicioColuna = coluna; 
    coluna += std::strlen(yytext); 
}
<COMMENT>"*/" {
    BEGIN(INITIAL); 
    coluna += std::strlen(yytext); 
} 
<COMMENT>"/*" {
    int next = yyinput(); // Lê o próximo caractere da entrada
    if (next == '\n')
    {
        yylineno--;
    }
    if (next == '/')
    {
        BEGIN(INITIAL);
        coluna += std::strlen(yytext) + 1;
    }
    else
    {
        result = "'" + std::string(yytext) + "' within block comment";
        throwException(ExceptionLevel::WARNING, false, yylineno, coluna, result);
        coluna += std::strlen(yytext);
        if (next == '\n')
        {
            yylineno++;
        }
        unput(next); // Devolve o caractere para o buffer de leitura
    }
}
<COMMENT>. { coluna += std::strlen(yytext); }
<COMMENT>\n { coluna = 1; }
<COMMENT><<EOF>> { 
    throwException(ExceptionLevel::ERROR, true, inicioLinha, inicioColuna, "unterminated comment");
    exit(1);
}
"//".*\n               {/*Ignora comentário */ }
\n                     {coluna=1;/*Ignora quebra de linha*/ }
[ ]                    {coluna++; }
"void"                 {print("VOID"); }
"int"                  {print("INT"); }
"char"                 {print("CHAR"); }
"return"               {print("RETURN"); }
"break"                {print("BREAK"); }
"switch"               {print("SWITCH"); }
"case"                 {print("CASE"); }
"default"              {print("DEFAULT"); }
"do"                   {print("DO"); }
"while"                {print("WHILE"); }
"for"                  {print("FOR"); }
"if"                   {print("IF"); }
"else"                 {print("ELSE"); }
"typedef"              {print("TYPEDEF"); }
"struct"               {print("STRUCT"); }
"+"                    {print("PLUS"); }
"-"                    {print("MINUS"); }
"*"                    {print("MULTIPLY"); }
"/"                    {print("DIV"); }
"%"                    {print("REMAINDER"); }
"++"                   {print("INC"); }
"--"                   {print("DEC"); }
"&"                    {print("BITWISE_AND"); }
"|"                    {print("BITWISE_OR"); }
"~"                    {print("BITWISE_NOT"); }
"^"                    {print("BITWISE_XOR"); }
"!"                    {print("NOT"); }
"&&"                   {print("LOGICAL_AND"); }
"||"                   {print("LOGICAL_OR"); }
"=="                   {print("EQUAL"); }
"!="                   {print("NOT_EQUAL"); }
"<"                    {print("LESS_THAN"); }
">"                    {print("GREATER_THAN"); }
"<="                   {print("LESS_EQUAL"); }
">="                   {print("GREATER_EQUAL"); }
">>"                   {print("R_SHIFT"); }
"<<"                   {print("L_SHIFT"); }
"="                    {print("ASSIGN"); }
"+="                   {print("ADD_ASSIGN"); }
"-="                   {print("MINUS_ASSIGN"); }
";"                    {print("SEMICOLON"); }
","                    {print("COMMA"); }
":"                    {print("COLON"); }
"("                    {print("L_PAREN"); }
")"                    {print("R_PAREN"); }
"{"                    {print("L_CURLY_BRACKET"); }
"}"                    {print("R_CURLY_BRACKET"); }
"["                    {print("L_SQUARE_BRACKET"); }
"]"                    {print("R_SQUARE_BRACKET"); }
"?"                    {print("TERNARY_CONDITIONAL"); }
"#"                    {print("NUMBER_SIGN"); }
"->"                   {print("POINTER"); }
"printf"               {print("PRINTF"); }
"scanf"                {print("SCANF"); }
"define"               {print("DEFINE"); }
"exit"                 {print("EXIT"); }
[a-zA-Z_][0-9a-zA-Z_]* {
    if (yyleng <= 255)
    {
        result = "IDENTIFIER(" + std::string(yytext) + ")";
        print(result);
    }
    else
    {
        throwException(ExceptionLevel::ERROR, true, yylineno, coluna, "identifier too long");
    }
}
0[1-7][0-7]*           {result = "NUM_OCTAL(" + std::string(yytext) + ")"; print(result); }
0[xX][0-9a-fA-F]+      {result = "NUM_HEXA(" + std::string(yytext) + ")"; print(result); }
[0-9]+                 {result = "NUM_INTEGER(" + std::string(yytext) + ")"; print(result); }
\"(\\.|[^"\\])*?\"     {result = "STRING(" + std::string(yytext+1, std::strlen(yytext) - 2) + ")"; print(result); }
\'(\\.|[^'\\])\'       {result = "CHARACTER(" + std::string(yytext+1, std::strlen(yytext) - 2) + ")"; print(result); }
.                      {throwException(ExceptionLevel::ERROR, true, yylineno, coluna, yytext); }
%%