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
"about"                         { yycolno += std::strlen(yytext); return ABOUT;}
"float"                         { yycolno += std::strlen(yytext); return FLOAT;}
"settings"                      { yycolno += std::strlen(yytext); return SETTINGS;}
"h"                             { yycolno += std::strlen(yytext); return H;}
"view"                          { yycolno += std::strlen(yytext); return VIEW;}
"plot"                          { yycolno += std::strlen(yytext); return PLOT;}
"show"                          { yycolno += std::strlen(yytext); return SHOW;}
"axis"                          { yycolno += std::strlen(yytext); return AXIS;}
"integral"                      { yycolno += std::strlen(yytext); return INTEGRAL;}
"steps"                         { yycolno += std::strlen(yytext); return STEPS;}
"precision"                     { yycolno += std::strlen(yytext); return PRECISION;}
"solve"                         { yycolno += std::strlen(yytext); return SOLVE;}
"connect"                       { yycolno += std::strlen(yytext); return CONNECT;}
"dots"                          { yycolno += std::strlen(yytext); return DOTS;}
"integrate"                     { yycolno += std::strlen(yytext); return INTEGRATE;}
"quit"                          { yycolno += std::strlen(yytext); return QUIT;}
"sum"                           { yycolno += std::strlen(yytext); return SUM;}
"linear"                        { yycolno += std::strlen(yytext); return LINEAR;}
"system"                        { yycolno += std::strlen(yytext); return SYSTEM;}
"reset"                         { yycolno += std::strlen(yytext); return RESET;}
"symbols"                       { yycolno += std::strlen(yytext); return SYMBOLS;}
"determinant"                   { yycolno += std::strlen(yytext); return DETERMINANT;}
"matrix"                        { yycolno += std::strlen(yytext); return MATRIX;}
"rpn"                           { yycolno += std::strlen(yytext); return RPN;}
"off"                           { yycolno += std::strlen(yytext); return OFF;}
"v"                             { yycolno += std::strlen(yytext); return V;}
"erase"                         { yycolno += std::strlen(yytext); return ERASE;}
"on"                            { yycolno += std::strlen(yytext); return ON;}
"set"                           { yycolno += std::strlen(yytext); return SET;}
":"                             { yycolno += std::strlen(yytext); return COLON;}
"="                             { yycolno += std::strlen(yytext); return EQUAL;}
":="                            { yycolno += std::strlen(yytext); return ASSIGN;}
"["                             { yycolno += std::strlen(yytext); return LEFT_BRACKET;}
"]"                             { yycolno += std::strlen(yytext); return RIGHT_BRACKET;}
";"                             { yycolno += std::strlen(yytext); return SEMICOLON;}
","                             { yycolno += std::strlen(yytext); return COMMA;}
[Pp][Ii]                        { yycolno += std::strlen(yytext); return PI_CONSTANT; }
[Ee]                            { yycolno += std::strlen(yytext); return EULER_CONSTANT; }
[xX]                            { yycolno += std::strlen(yytext); return VARIABLE; }
[0-9]+                          { yylval.double_val = atof(yytext); yycolno += std::strlen(yytext); return INTEGER; }
[0-9]+"."[0-9]+                 { yylval.double_val = atof(yytext); yycolno += std::strlen(yytext); return REAL_NUMBER; }
[a-zA-Z]+[_0-9a-zA-Z]*          { yycolno += std::strlen(yytext); return IDENTIFIER; }
.                               { throwException(ExceptionLevel::ERROR, ExceptionType::LEXICAL, yylineno, yycolno, yytext); }
%%