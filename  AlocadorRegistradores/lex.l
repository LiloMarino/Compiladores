%option noyywrap
%option yylineno
%{
#include <iostream>
#include <string>
#include "syntax.tab.h"

%}
%%

[ \t\n]        {}
"Grafo"        { return GRAFO; }
"-->"          { return ARROW; }
":"            { return COLON; }
"="            { return ASSIGN; }
"K"            { return K; }
[0-9]+         { yylval.integer = atof(yytext); return INTEGER; }
.              { std::cout << "Invalid Symbol: " << yytext << std::endl; }

%%