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
int yycolno = 1;
int inicioLinha;
int inicioColuna;
std::string result;

%}
%%

"/*" { 
    BEGIN(COMMENT); 
    inicioLinha = yylineno;
    inicioColuna = yycolno; 
    yycolno += std::strlen(yytext); 
}
<COMMENT>"*/" {
    BEGIN(INITIAL); 
    yycolno += std::strlen(yytext); 
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
        yycolno += std::strlen(yytext) + 1;
    } 
    else 
    {
        result = "'" + std::string(yytext) + "' within block comment";
        throwException(ExceptionLevel::WARNING, ExceptionType::NONE, yylineno, yycolno, result);
        yycolno += std::strlen(yytext);
        if (next == '\n') 
        { 
            yylineno++; 
        }
        unput(next); 
    }
}
<COMMENT>.             { yycolno += std::strlen(yytext); }
<COMMENT>\n            { yycolno = 1; }
<COMMENT><<EOF>> { 
    throwException(ExceptionLevel::ERROR, ExceptionType::LEXICAL, inicioLinha, inicioColuna, "unterminated comment");
    exit(1);
}
"//".*\n               { /* Ignora comentário de linha */ }
\n                     { yycolno = 1; /* Ignora quebra de linha */ }
[ ]                    { yycolno++;  /* Ignora espaço em branco */ }
"void"                 { yycolno += std::strlen(yytext); return VOID; }
"int"                  { yycolno += std::strlen(yytext); return INT; }
"char"                 { yycolno += std::strlen(yytext); return CHAR; }
"return"               { yycolno += std::strlen(yytext); return RETURN; }
"break"                { yycolno += std::strlen(yytext); return BREAK; }
"switch"               { yycolno += std::strlen(yytext); return SWITCH; }
"case"                 { yycolno += std::strlen(yytext); return CASE; }
"default"              { yycolno += std::strlen(yytext); return DEFAULT; }
"do"                   { yycolno += std::strlen(yytext); return DO; }
"while"                { yycolno += std::strlen(yytext); return WHILE; }
"for"                  { yycolno += std::strlen(yytext); return FOR; }
"if"                   { yycolno += std::strlen(yytext); return IF; }
"else"                 { yycolno += std::strlen(yytext); return ELSE; }
"typedef"              { yycolno += std::strlen(yytext); return TYPEDEF; }
"struct"               { yycolno += std::strlen(yytext); return STRUCT; }
"+"                    { yycolno += std::strlen(yytext); return PLUS; }
"-"                    { yycolno += std::strlen(yytext); return MINUS; }
"*"                    { yycolno += std::strlen(yytext); return MULTIPLY; }
"/"                    { yycolno += std::strlen(yytext); return DIVIDE; }
"%"                    { yycolno += std::strlen(yytext); return MODULO; }
"++"                   { yycolno += std::strlen(yytext); return INCREMENT; }
"--"                   { yycolno += std::strlen(yytext); return DECREMENT; }
"&"                    { yycolno += std::strlen(yytext); return BITWISE_AND; }
"|"                    { yycolno += std::strlen(yytext); return BITWISE_OR; }
"~"                    { yycolno += std::strlen(yytext); return BITWISE_NOT; }
"^"                    { yycolno += std::strlen(yytext); return BITWISE_XOR; }
"!"                    { yycolno += std::strlen(yytext); return NOT; }
"&&"                   { yycolno += std::strlen(yytext); return LOGICAL_AND; }
"||"                   { yycolno += std::strlen(yytext); return LOGICAL_OR; }
"=="                   { yycolno += std::strlen(yytext); return EQUAL; }
"!="                   { yycolno += std::strlen(yytext); return NOT_EQUAL; }
"<"                    { yycolno += std::strlen(yytext); return LESS_THAN; }
">"                    { yycolno += std::strlen(yytext); return GREATER_THAN; }
"<="                   { yycolno += std::strlen(yytext); return LESS_EQUAL; }
">="                   { yycolno += std::strlen(yytext); return GREATER_EQUAL; }
">>"                   { yycolno += std::strlen(yytext); return RIGHT_SHIFT; }
"<<"                   { yycolno += std::strlen(yytext); return LEFT_SHIFT; }
"="                    { yycolno += std::strlen(yytext); return ASSIGN; }
"+="                   { yycolno += std::strlen(yytext); return PLUS_ASSIGN; }
"-="                   { yycolno += std::strlen(yytext); return MINUS_ASSIGN; }
";"                    { yycolno += std::strlen(yytext); return SEMICOLON; }
","                    { yycolno += std::strlen(yytext); return COMMA; }
":"                    { yycolno += std::strlen(yytext); return COLON; }
"("                    { yycolno += std::strlen(yytext); return LEFT_PAREN; }
")"                    { yycolno += std::strlen(yytext); return RIGHT_PAREN; }
"{"                    { yycolno += std::strlen(yytext); return LEFT_CURLY_BRACKET; }
"}"                    { yycolno += std::strlen(yytext); return RIGHT_CURLY_BRACKET; }
"["                    { yycolno += std::strlen(yytext); return LEFT_SQUARE_BRACKET; }
"]"                    { yycolno += std::strlen(yytext); return RIGHT_SQUARE_BRACKET; }
"?"                    { yycolno += std::strlen(yytext); return QUESTION_MARK; }
"#"                    { yycolno += std::strlen(yytext); return HASH; }
"->"                   { yycolno += std::strlen(yytext); return ARROW; }
"printf"               { yycolno += std::strlen(yytext); return PRINTF; }
"scanf"                { yycolno += std::strlen(yytext); return SCANF; }
"define"               { yycolno += std::strlen(yytext); return DEFINE; }
"exit"                 { yycolno += std::strlen(yytext); return EXIT; }
[a-zA-Z_][0-9a-zA-Z_]* {
    if (yyleng <= 255) {
        yycolno += std::strlen(yytext);
        return IDENTIFIER;
    } else {
        throwException(ExceptionLevel::ERROR, ExceptionType::LEXICAL, yylineno, yycolno, "identifier too long");
    }
}
0[1-7][0-7]*           { yycolno += std::strlen(yytext); return NUM_OCTAL; }
0[xX][0-9a-fA-F]+      { yycolno += std::strlen(yytext); return NUM_HEXA; }
[0-9]+                 { yycolno += std::strlen(yytext); return NUM_INTEGER; }
\"(\\.|[^"\\])*?\"     { yycolno += std::strlen(yytext); return STRING_LITERAL; }
\'(\\.|[^'\\])\'       { yycolno += std::strlen(yytext); return CHARACTER_LITERAL; }
.                      { throwException(ExceptionLevel::ERROR, ExceptionType::LEXICAL, yylineno, yycolno, yytext); }
%%