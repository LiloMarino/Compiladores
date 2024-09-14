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
std::string temp;

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
        throwException(ExceptionLevel::WARNING, ExceptionType::NONE, yylineno, yycolno, "'" + std::string(yytext) + "' within block comment");
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
}
<<EOF>>                { return EOF_TOKEN; }
"//".*                 { /* Ignora comentário de linha */ }
\n                     { addLine(temp); temp.clear(); yycolno = 1; }
[ ]                    { temp += std::string(yytext); yycolno++; }
"void"                 { temp += std::string(yytext); yycolno += std::strlen(yytext); return VOID; }
"int"                  { temp += std::string(yytext); yycolno += std::strlen(yytext); return INT; }
"char"                 { temp += std::string(yytext); yycolno += std::strlen(yytext); return CHAR; }
"return"               { temp += std::string(yytext); yycolno += std::strlen(yytext); return RETURN; }
"break"                { temp += std::string(yytext); yycolno += std::strlen(yytext); return BREAK; }
"switch"               { temp += std::string(yytext); yycolno += std::strlen(yytext); return SWITCH; }
"case"                 { temp += std::string(yytext); yycolno += std::strlen(yytext); return CASE; }
"default"              { temp += std::string(yytext); yycolno += std::strlen(yytext); return DEFAULT; }
"do"                   { temp += std::string(yytext); yycolno += std::strlen(yytext); return DO; }
"while"                { temp += std::string(yytext); yycolno += std::strlen(yytext); return WHILE; }
"for"                  { temp += std::string(yytext); yycolno += std::strlen(yytext); return FOR; }
"if"                   { temp += std::string(yytext); yycolno += std::strlen(yytext); return IF; }
"else"                 { temp += std::string(yytext); yycolno += std::strlen(yytext); return ELSE; }
"typedef"              { temp += std::string(yytext); yycolno += std::strlen(yytext); return TYPEDEF; }
"struct"               { temp += std::string(yytext); yycolno += std::strlen(yytext); return STRUCT; }
"+"                    { temp += std::string(yytext); yycolno += std::strlen(yytext); return PLUS; }
"-"                    { temp += std::string(yytext); yycolno += std::strlen(yytext); return MINUS; }
"*"                    { temp += std::string(yytext); yycolno += std::strlen(yytext); return MULTIPLY; }
"/"                    { temp += std::string(yytext); yycolno += std::strlen(yytext); return DIVIDE; }
"%"                    { temp += std::string(yytext); yycolno += std::strlen(yytext); return MODULO; }
"++"                   { temp += std::string(yytext); yycolno += std::strlen(yytext); return INCREMENT; }
"--"                   { temp += std::string(yytext); yycolno += std::strlen(yytext); return DECREMENT; }
"&"                    { temp += std::string(yytext); yycolno += std::strlen(yytext); return BITWISE_AND; }
"|"                    { temp += std::string(yytext); yycolno += std::strlen(yytext); return BITWISE_OR; }
"~"                    { temp += std::string(yytext); yycolno += std::strlen(yytext); return BITWISE_NOT; }
"^"                    { temp += std::string(yytext); yycolno += std::strlen(yytext); return BITWISE_XOR; }
"!"                    { temp += std::string(yytext); yycolno += std::strlen(yytext); return NOT; }
"&&"                   { temp += std::string(yytext); yycolno += std::strlen(yytext); return LOGICAL_AND; }
"||"                   { temp += std::string(yytext); yycolno += std::strlen(yytext); return LOGICAL_OR; }
"=="                   { temp += std::string(yytext); yycolno += std::strlen(yytext); return EQUAL; }
"!="                   { temp += std::string(yytext); yycolno += std::strlen(yytext); return NOT_EQUAL; }
"<"                    { temp += std::string(yytext); yycolno += std::strlen(yytext); return LESS_THAN; }
">"                    { temp += std::string(yytext); yycolno += std::strlen(yytext); return GREATER_THAN; }
"<="                   { temp += std::string(yytext); yycolno += std::strlen(yytext); return LESS_EQUAL; }
">="                   { temp += std::string(yytext); yycolno += std::strlen(yytext); return GREATER_EQUAL; }
">>"                   { temp += std::string(yytext); yycolno += std::strlen(yytext); return RIGHT_SHIFT; }
"<<"                   { temp += std::string(yytext); yycolno += std::strlen(yytext); return LEFT_SHIFT; }
"="                    { temp += std::string(yytext); yycolno += std::strlen(yytext); return ASSIGN; }
"+="                   { temp += std::string(yytext); yycolno += std::strlen(yytext); return PLUS_ASSIGN; }
"-="                   { temp += std::string(yytext); yycolno += std::strlen(yytext); return MINUS_ASSIGN; }
";"                    { temp += std::string(yytext); yycolno += std::strlen(yytext); return SEMICOLON; }
","                    { temp += std::string(yytext); yycolno += std::strlen(yytext); return COMMA; }
":"                    { temp += std::string(yytext); yycolno += std::strlen(yytext); return COLON; }
"("                    { temp += std::string(yytext); yycolno += std::strlen(yytext); return LEFT_PAREN; }
")"                    { temp += std::string(yytext); yycolno += std::strlen(yytext); return RIGHT_PAREN; }
"{"                    { temp += std::string(yytext); yycolno += std::strlen(yytext); return LEFT_CURLY_BRACKET; }
"}"                    { temp += std::string(yytext); yycolno += std::strlen(yytext); return RIGHT_CURLY_BRACKET; }
"["                    { temp += std::string(yytext); yycolno += std::strlen(yytext); return LEFT_SQUARE_BRACKET; }
"]"                    { temp += std::string(yytext); yycolno += std::strlen(yytext); return RIGHT_SQUARE_BRACKET; }
"?"                    { temp += std::string(yytext); yycolno += std::strlen(yytext); return QUESTION_MARK; }
"#"                    { temp += std::string(yytext); yycolno += std::strlen(yytext); return HASH; }
"->"                   { temp += std::string(yytext); yycolno += std::strlen(yytext); return ARROW; }
"printf"               { temp += std::string(yytext); yycolno += std::strlen(yytext); return PRINTF; }
"scanf"                { temp += std::string(yytext); yycolno += std::strlen(yytext); return SCANF; }
"define"               { temp += std::string(yytext); yycolno += std::strlen(yytext); return DEFINE; }
"exit"                 { temp += std::string(yytext); yycolno += std::strlen(yytext); return EXIT; }
[a-zA-Z_][0-9a-zA-Z_]* {
    if (yyleng <= 255) {
        temp += std::string(yytext);
        yycolno += std::strlen(yytext);
        return IDENTIFIER;
    } else {
        throwException(ExceptionLevel::ERROR, ExceptionType::LEXICAL, yylineno, yycolno, "identifier too long");
    }
}
0[1-7][0-7]*           { temp += std::string(yytext); yycolno += std::strlen(yytext); return NUM_OCTAL; }
0[xX][0-9a-fA-F]+      { temp += std::string(yytext); yycolno += std::strlen(yytext); return NUM_HEXA; }
[0-9]+                 { temp += std::string(yytext); yycolno += std::strlen(yytext); return NUM_INTEGER; }
\"(\\.|[^"\\])*?\"     { temp += std::string(yytext); yycolno += std::strlen(yytext); return STRING_LITERAL; }
\'(\\.|[^'\\])\'       { temp += std::string(yytext); yycolno += std::strlen(yytext); return CHARACTER_LITERAL; }
.                      { throwException(ExceptionLevel::ERROR, ExceptionType::LEXICAL, yylineno, yycolno, yytext); }
%%