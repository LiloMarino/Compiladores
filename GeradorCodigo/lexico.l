%option noyywrap
%option yylineno
%{
#include <iostream>
#include <string>
#include "sintatico.tab.h"

%}
%%

[ \t\n]                     {}
"AST -->"                   {}
"GLOBAL"                    { return GLOBAL; }
"VARIABLE"                  { return VARIABLE; }
"CONSTANT"                  { return CONSTANT; }
"PARAMETER"                 { return PARAMETER; }
"VALUE"                     { return VALUE; }
"RETURN_TYPE"               { return RETURN_TYPE; }
"TYPE"                      { return TYPE; }
"void"                      { return VOID; }
"int"                       { return INT; } 
"char"                      { return CHAR; }
"FUNCTION"                  { return FUNCTION; }
"END_FUNCTION"              { return END_FUNCTION; }
"RETURN"                    { return RETURN; }
"DO_WHILE"                  { return DO_WHILE; }
"WHILE"                     { return WHILE; }
"FOR"                       { return FOR; }
"IF"                        { return IF; }
"PRINTF"                    { return PRINTF; }
"SCANF"                     { return SCANF; }
"EXIT"                      { return EXIT; }
"+"                         { return PLUS; }
"-"                         { return MINUS; }
"*"                         { return MULTIPLY; }
"/"                         { return DIVIDE; }
"%"                         { return REMAINDER; }
"++"                        { return INC; }
"--"                        { return DEC; }
"&"                         { return BITWISE_AND; }
"|"                         { return BITWISE_OR; }
"~"                         { return BITWISE_NOT; }
"^"                         { return BITWISE_XOR; }
"!"                         { return NOT; }
"&&"                        { return LOGICAL_AND; }
"||"                        { return LOGICAL_OR; }
"=="                        { return EQUAL; }
"!="                        { return NOT_EQUAL; }
"<"                         { return LESS_THAN; }
">"                         { return GREATER_THAN; }
"<="                        { return LESS_EQUAL; }
">="                        { return GREATER_EQUAL; }
">>"                        { return R_SHIFT; }
"<<"                        { return L_SHIFT; }
"="                         { return ASSIGN; }
"+="                        { return ADD_ASSIGN; }
"-="                        { return MINUS_ASSIGN; }
";"                         { return SEMICOLON; }
","                         { return COMMA; }
":"                         { return COLON; }
"("                         { return L_PAREN; }
")"                         { return R_PAREN; }
"["                         { return L_SQUARE_BRACKET; }
"]"                         { return R_SQUARE_BRACKET; }
"?"                         { return TERNARY_CONDITIONAL; }
<<EOF>>                     { return EOF_TOKEN; }
\"([^\\\"]|\\.)*\"          { yylval.str = new std::string(yytext); return STRING; }
(_*[a-zA-Z][a-zA-Z0-9_]*)   { yylval.str = new std::string(yytext); return IDENTIFIER; }
"'"(\\.|[^\\']){1}"'"       { yylval.str = new std::string(yytext); return CHARACTER; }
[0-9]+                      { yylval.value = atof(yytext); return INTEGER; }
.                           { std::cout << "Invalid Symbol: " << yytext << std::endl; }

%%