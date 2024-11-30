%option noyywrap
%option yylineno
%{
#include <iostream>
#include <string>
#include <cstring>
#include "utils.hpp"
#include "sintatico.tab.h"

%}
%%

[ \t\n]                         {  }
"+"                             { return PLUS; }
"-"                             { return MINUS; }
"*"                             { return MULTIPLY; }
"/"                             { return DIVIDE; }
"^"                             { return EXPONENT; }
"%"                             { return MODULO; }
"("                             { return LEFT_PAREN; }
")"                             { return RIGHT_PAREN; }
("sin"|"sen")                   { return SIN; }
"cos"                           { return COS; }
"tan"                           { return TAN; }
"abs"                           { return ABS; }
"about"                         { return ABOUT;}
"float"                         { return FLOAT;}
"settings"                      { return SETTINGS;}
"h_view"                        { return H_VIEW;}
"plot"                          { return PLOT;}
"show"                          { return SHOW;}
"axis"                          { return AXIS;}
"integral_steps"                { return INTEGRAL_STEPS;}
"precision"                     { return PRECISION;}
"solve"                         { return SOLVE;}
"connect_dots"                  { return CONNECT_DOTS;}
"integrate"                     { return INTEGRATE;}
"quit"                          { return QUIT;}
"sum"                           { return SUM;}
"linear_system"                 { return LINEAR_SYSTEM;}
"reset"                         { return RESET;}
"symbols"                       { return SYMBOLS;}
"determinant"                   { return DETERMINANT;}
"matrix"                        { return MATRIX;}
"rpn"                           { return RPN;}
"off"                           { return OFF;}
"v_view"                        { return V_VIEW;}
"erase"                         { return ERASE;}
"on"                            { return ON;}
"set"                           { return SET;}
":"                             { return COLON;}
"="                             { return EQUAL;}
":="                            { return ASSIGN;}
"["                             { return LEFT_BRACKET;}
"]"                             { return RIGHT_BRACKET;}
";"                             { return SEMICOLON;}
","                             { return COMMA;}
[Pp][Ii]                        { return PI_CONSTANT; }
[Ee]                            { return EULER_CONSTANT; }
[xX]                            { return X; }
[0-9]+                          { yylval.value = atof(yytext); return INTEGER; }
[0-9]+"."[0-9]+                 { yylval.value = atof(yytext); return REAL_NUMBER; }
[a-zA-Z]+[_0-9a-zA-Z]*          { yylval.str = new std::string(yytext); return IDENTIFIER; }
.                               {  }
%%