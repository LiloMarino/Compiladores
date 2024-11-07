%option noyywrap
%option yylineno
%{
#include <iostream>
#include <string>
#include <cstring>
#include "utils.hpp"
#include "sintatico.tab.h"

// Variáveis globais
int yycolno = 1;

%}
%%

\n                              { yycolno = 1; }
[ ]                             { yycolno++; }
"+"                             { yycolno += std::strlen(yytext); return PLUS; }
"-"                             { yycolno += std::strlen(yytext); return MINUS; }
"*"                             { yycolno += std::strlen(yytext); return MULTIPLY; }
"/"                             { yycolno += std::strlen(yytext); return DIVIDE; }
"^"                             { yycolno += std::strlen(yytext); return EXPONENT; }
"%"                             { yycolno += std::strlen(yytext); return MODULO; }
"("                             { yycolno += std::strlen(yytext); return LEFT_PAREN; }
")"                             { yycolno += std::strlen(yytext); return RIGHT_PAREN; }
("sin"|"sen")                   { yycolno += std::strlen(yytext); return SIN; }
"cos"                           { yycolno += std::strlen(yytext); return COS; }
"tan"                           { yycolno += std::strlen(yytext); return TAN; }
"abs"                           { yycolno += std::strlen(yytext); return ABS; }
[0-9]+                          { yycolno += std::strlen(yytext); return INTEGER; }
[0-9]+"."[0-9]+                 { yycolno += std::strlen(yytext); return REAL_NUMBER; }
[Pp][Ii]                        { yycolno += std::strlen(yytext); return PI_CONSTANT; }
[Ee]                            { yycolno += std::strlen(yytext); return EULER_CONSTANT; }
"x"                             { yycolno += std::strlen(yytext); return VARIABLE; }
[a-zA-Z]+[_0-9a-zA-Z]*          { yycolno += std::strlen(yytext); return IDENTIFIER; }
.                               { throwException(ExceptionLevel::ERROR, ExceptionType::LEXICAL, yylineno, yycolno, yytext); }
%%