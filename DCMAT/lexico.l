%option noyywrap
%option yylineno
%{
#include <iostream>
#include <string>
#include <memory>
#include "sintatico.tab.h"
std::unique_ptr<std::string> activeToken = nullptr;

%}
%%

[ \t]                           {  }
\n                              { activeToken = std::make_unique<std::string>(yytext); return EOL; }
"+"                             { activeToken = std::make_unique<std::string>(yytext); return PLUS; }
"-"                             { activeToken = std::make_unique<std::string>(yytext); return MINUS; }
"*"                             { activeToken = std::make_unique<std::string>(yytext); return MULTIPLY; }
"/"                             { activeToken = std::make_unique<std::string>(yytext); return DIVIDE; }
"^"                             { activeToken = std::make_unique<std::string>(yytext); return EXPONENT; }
"%"                             { activeToken = std::make_unique<std::string>(yytext); return MODULO; }
"("                             { activeToken = std::make_unique<std::string>(yytext); return LEFT_PAREN; }
")"                             { activeToken = std::make_unique<std::string>(yytext); return RIGHT_PAREN; }
("sin"|"sen")                   { activeToken = std::make_unique<std::string>(yytext); return SIN; }
"cos"                           { activeToken = std::make_unique<std::string>(yytext); return COS; }
"tan"                           { activeToken = std::make_unique<std::string>(yytext); return TAN; }
"abs"                           { activeToken = std::make_unique<std::string>(yytext); return ABS; }
"about"                         { activeToken = std::make_unique<std::string>(yytext); return ABOUT;}
"float"                         { activeToken = std::make_unique<std::string>(yytext); return FLOAT;}
"settings"                      { activeToken = std::make_unique<std::string>(yytext); return SETTINGS;}
"h_view"                        { activeToken = std::make_unique<std::string>(yytext); return H_VIEW;}
"plot"                          { activeToken = std::make_unique<std::string>(yytext); return PLOT;}
"show"                          { activeToken = std::make_unique<std::string>(yytext); return SHOW;}
"axis"                          { activeToken = std::make_unique<std::string>(yytext); return AXIS;}
"integral_steps"                { activeToken = std::make_unique<std::string>(yytext); return INTEGRAL_STEPS;}
"precision"                     { activeToken = std::make_unique<std::string>(yytext); return PRECISION;}
"solve"                         { activeToken = std::make_unique<std::string>(yytext); return SOLVE;}
"connect_dots"                  { activeToken = std::make_unique<std::string>(yytext); return CONNECT_DOTS;}
"integrate"                     { activeToken = std::make_unique<std::string>(yytext); return INTEGRATE;}
"quit"                          { activeToken = std::make_unique<std::string>(yytext); return QUIT;}
"sum"                           { activeToken = std::make_unique<std::string>(yytext); return SUM;}
"linear_system"                 { activeToken = std::make_unique<std::string>(yytext); return LINEAR_SYSTEM;}
"reset"                         { activeToken = std::make_unique<std::string>(yytext); return RESET;}
"symbols"                       { activeToken = std::make_unique<std::string>(yytext); return SYMBOLS;}
"determinant"                   { activeToken = std::make_unique<std::string>(yytext); return DETERMINANT;}
"matrix"                        { activeToken = std::make_unique<std::string>(yytext); return MATRIX;}
"rpn"                           { activeToken = std::make_unique<std::string>(yytext); return RPN;}
"off"                           { activeToken = std::make_unique<std::string>(yytext); return OFF;}
"v_view"                        { activeToken = std::make_unique<std::string>(yytext); return V_VIEW;}
"erase"                         { activeToken = std::make_unique<std::string>(yytext); return ERASE;}
"on"                            { activeToken = std::make_unique<std::string>(yytext); return ON;}
"set"                           { activeToken = std::make_unique<std::string>(yytext); return SET;}
":"                             { activeToken = std::make_unique<std::string>(yytext); return COLON;}
"="                             { activeToken = std::make_unique<std::string>(yytext); return EQUAL;}
":="                            { activeToken = std::make_unique<std::string>(yytext); return ASSIGN;}
"["                             { activeToken = std::make_unique<std::string>(yytext); return LEFT_BRACKET;}
"]"                             { activeToken = std::make_unique<std::string>(yytext); return RIGHT_BRACKET;}
";"                             { activeToken = std::make_unique<std::string>(yytext); return SEMICOLON;}
","                             { activeToken = std::make_unique<std::string>(yytext); return COMMA;}
[Pp][Ii]                        { activeToken = std::make_unique<std::string>(yytext); return PI_CONSTANT; }
[Ee]                            { activeToken = std::make_unique<std::string>(yytext); return EULER_CONSTANT; }
[xX]                            { activeToken = std::make_unique<std::string>(yytext); return X; }
[0-9]+                          { activeToken = std::make_unique<std::string>(yytext); yylval.value = atof(yytext); return INTEGER; }
[0-9]+"."[0-9]+                 { activeToken = std::make_unique<std::string>(yytext); yylval.value = atof(yytext); return REAL_NUMBER; }
[a-zA-Z]+[_0-9a-zA-Z]*          { activeToken = std::make_unique<std::string>(yytext); yylval.str = new std::string(yytext); return IDENTIFIER; }
.                               { std::cout << "Invalid Symbol: " << yytext << std::endl; }
%%