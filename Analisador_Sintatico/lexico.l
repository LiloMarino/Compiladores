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
<COMMENT>.             { coluna += std::strlen(yytext); }
<COMMENT>\n            { coluna = 1; }
<COMMENT><<EOF>> { 
    throwException(ExceptionLevel::ERROR, true, inicioLinha, inicioColuna, "unterminated comment");
    exit(1);
}
"//".*\n               {/*Ignora comentário */ }
\n                     {coluna = 1; }
[ ]                    {coluna++; }
"void"                 {coluna += std::strlen(yytext); }
"int"                  {coluna += std::strlen(yytext); }
"char"                 {coluna += std::strlen(yytext); }
"return"               {coluna += std::strlen(yytext); }
"break"                {coluna += std::strlen(yytext); }
"switch"               {coluna += std::strlen(yytext); }
"case"                 {coluna += std::strlen(yytext); }
"default"              {coluna += std::strlen(yytext); }
"do"                   {coluna += std::strlen(yytext); }
"while"                {coluna += std::strlen(yytext); }
"for"                  {coluna += std::strlen(yytext); }
"if"                   {coluna += std::strlen(yytext); }
"else"                 {coluna += std::strlen(yytext); }
"typedef"              {coluna += std::strlen(yytext); }
"struct"               {coluna += std::strlen(yytext); }
"+"                    {coluna += std::strlen(yytext); }
"-"                    {coluna += std::strlen(yytext); }
"*"                    {coluna += std::strlen(yytext); }
"/"                    {coluna += std::strlen(yytext); }
"%"                    {coluna += std::strlen(yytext); }
"++"                   {coluna += std::strlen(yytext); }
"--"                   {coluna += std::strlen(yytext); }
"&"                    {coluna += std::strlen(yytext); }
"|"                    {coluna += std::strlen(yytext); }
"~"                    {coluna += std::strlen(yytext); }
"^"                    {coluna += std::strlen(yytext); }
"!"                    {coluna += std::strlen(yytext); }
"&&"                   {coluna += std::strlen(yytext); }
"||"                   {coluna += std::strlen(yytext); }
"=="                   {coluna += std::strlen(yytext); }
"!="                   {coluna += std::strlen(yytext); }
"<"                    {coluna += std::strlen(yytext); }
">"                    {coluna += std::strlen(yytext); }
"<="                   {coluna += std::strlen(yytext); }
">="                   {coluna += std::strlen(yytext); }
">>"                   {coluna += std::strlen(yytext); }
"<<"                   {coluna += std::strlen(yytext); }
"="                    {coluna += std::strlen(yytext); }
"+="                   {coluna += std::strlen(yytext); }
"-="                   {coluna += std::strlen(yytext); }
";"                    {coluna += std::strlen(yytext); }
","                    {coluna += std::strlen(yytext); }
":"                    {coluna += std::strlen(yytext); }
"("                    {coluna += std::strlen(yytext); }
")"                    {coluna += std::strlen(yytext); }
"{"                    {coluna += std::strlen(yytext); }
"}"                    {coluna += std::strlen(yytext); }
"["                    {coluna += std::strlen(yytext); }
"]"                    {coluna += std::strlen(yytext); }
"?"                    {coluna += std::strlen(yytext); }
"#"                    {coluna += std::strlen(yytext); }
"->"                   {coluna += std::strlen(yytext); }
"printf"               {coluna += std::strlen(yytext); }
"scanf"                {coluna += std::strlen(yytext); }
"define"               {coluna += std::strlen(yytext); }
"exit"                 {coluna += std::strlen(yytext); }
[a-zA-Z_][0-9a-zA-Z_]* {
    if (yyleng <= 255)
    {
        coluna += std::strlen(yytext);
    }
    else
    {
        throwException(ExceptionLevel::ERROR, true, yylineno, coluna, "identifier too long");
    }
}
0[1-7][0-7]*           { coluna += std::strlen(yytext); }
0[xX][0-9a-fA-F]+      { coluna += std::strlen(yytext); }
[0-9]+                 { coluna += std::strlen(yytext); }
\"(\\.|[^"\\])*?\"     { coluna += std::strlen(yytext); }
\'(\\.|[^'\\])\'       { coluna += std::strlen(yytext); }
.                      {throwException(ExceptionLevel::ERROR, true, yylineno, coluna, yytext); }
%%