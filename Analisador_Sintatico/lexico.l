%option noyywrap
%option yylineno
%x COMMENT
%{
#include <iostream>
#include <string>
#include <cstring>
#include "utils.hpp"
#include "sintatico.tab.h"

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
    int next = yyinput(); 
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
        unput(next); 
    }
}
<COMMENT>.             { coluna += std::strlen(yytext); }
<COMMENT>\n            { coluna = 1; }
<COMMENT><<EOF>> { 
    throwException(ExceptionLevel::ERROR, true, inicioLinha, inicioColuna, "unterminated comment");
    exit(1);
}
"//".*\n               { /* Ignora comentário de linha */ }
\n                     { coluna = 1; /* Ignora quebra de linha */ }
[ ]                    { coluna++;  /* Ignora espaço em branco */ }
"void"                 { coluna += std::strlen(yytext); return VOID; }
"int"                  { coluna += std::strlen(yytext); return INT; }
"char"                 { coluna += std::strlen(yytext); return CHAR; }
"return"               { coluna += std::strlen(yytext); return RETURN; }
"break"                { coluna += std::strlen(yytext); return BREAK; }
"switch"               { coluna += std::strlen(yytext); return SWITCH; }
"case"                 { coluna += std::strlen(yytext); return CASE; }
"default"              { coluna += std::strlen(yytext); return DEFAULT; }
"do"                   { coluna += std::strlen(yytext); return DO; }
"while"                { coluna += std::strlen(yytext); return WHILE; }
"for"                  { coluna += std::strlen(yytext); return FOR; }
"if"                   { coluna += std::strlen(yytext); return IF; }
"else"                 { coluna += std::strlen(yytext); return ELSE; }
"typedef"              { coluna += std::strlen(yytext); return TYPEDEF; }
"struct"               { coluna += std::strlen(yytext); return STRUCT; }
"+"                    { coluna += std::strlen(yytext); return PLUS; }
"-"                    { coluna += std::strlen(yytext); return MINUS; }
"*"                    { coluna += std::strlen(yytext); return MULTIPLY; }
"/"                    { coluna += std::strlen(yytext); return DIVIDE; }
"%"                    { coluna += std::strlen(yytext); return MODULO; }
"++"                   { coluna += std::strlen(yytext); return INCREMENT; }
"--"                   { coluna += std::strlen(yytext); return DECREMENT; }
"&"                    { coluna += std::strlen(yytext); return BITWISE_AND; }
"|"                    { coluna += std::strlen(yytext); return BITWISE_OR; }
"~"                    { coluna += std::strlen(yytext); return BITWISE_NOT; }
"^"                    { coluna += std::strlen(yytext); return BITWISE_XOR; }
"!"                    { coluna += std::strlen(yytext); return NOT; }
"&&"                   { coluna += std::strlen(yytext); return LOGICAL_AND; }
"||"                   { coluna += std::strlen(yytext); return LOGICAL_OR; }
"=="                   { coluna += std::strlen(yytext); return EQUAL; }
"!="                   { coluna += std::strlen(yytext); return NOT_EQUAL; }
"<"                    { coluna += std::strlen(yytext); return LESS_THAN; }
">"                    { coluna += std::strlen(yytext); return GREATER_THAN; }
"<="                   { coluna += std::strlen(yytext); return LESS_EQUAL; }
">="                   { coluna += std::strlen(yytext); return GREATER_EQUAL; }
">>"                   { coluna += std::strlen(yytext); return RIGHT_SHIFT; }
"<<"                   { coluna += std::strlen(yytext); return LEFT_SHIFT; }
"="                    { coluna += std::strlen(yytext); return ASSIGN; }
"+="                   { coluna += std::strlen(yytext); return PLUS_ASSIGN; }
"-="                   { coluna += std::strlen(yytext); return MINUS_ASSIGN; }
";"                    { coluna += std::strlen(yytext); return SEMICOLON; }
","                    { coluna += std::strlen(yytext); return COMMA; }
":"                    { coluna += std::strlen(yytext); return COLON; }
"("                    { coluna += std::strlen(yytext); return LEFT_PAREN; }
")"                    { coluna += std::strlen(yytext); return RIGHT_PAREN; }
"{"                    { coluna += std::strlen(yytext); return LEFT_CURLY_BRACKET; }
"}"                    { coluna += std::strlen(yytext); return RIGHT_CURLY_BRACKET; }
"["                    { coluna += std::strlen(yytext); return LEFT_SQUARE_BRACKET; }
"]"                    { coluna += std::strlen(yytext); return RIGHT_SQUARE_BRACKET; }
"?"                    { coluna += std::strlen(yytext); return QUESTION_MARK; }
"#"                    { coluna += std::strlen(yytext); return HASH; }
"->"                   { coluna += std::strlen(yytext); return ARROW; }
"printf"               { coluna += std::strlen(yytext); return PRINTF; }
"scanf"                { coluna += std::strlen(yytext); return SCANF; }
"define"               { coluna += std::strlen(yytext); return DEFINE; }
"exit"                 { coluna += std::strlen(yytext); return EXIT; }
[a-zA-Z_][0-9a-zA-Z_]* {
    if (yyleng <= 255) {
        coluna += std::strlen(yytext);
        return IDENTIFIER;
    } else {
        throwException(ExceptionLevel::ERROR, true, yylineno, coluna, "identifier too long");
    }
}
0[1-7][0-7]*           { coluna += std::strlen(yytext); return NUM_OCTAL; }
0[xX][0-9a-fA-F]+      { coluna += std::strlen(yytext); return NUM_HEXA; }
[0-9]+                 { coluna += std::strlen(yytext); return NUM_INTEGER; }
\"(\\.|[^"\\])*?\"     { coluna += std::strlen(yytext); return STRING_LITERAL; }
\'(\\.|[^'\\])\'       { coluna += std::strlen(yytext); return CHARACTER_LITERAL; }
.                      { throwException(ExceptionLevel::ERROR, true, yylineno, coluna, yytext); }
%%