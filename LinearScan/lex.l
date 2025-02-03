%option noyywrap
%option yylineno
%{
#include <iostream>
#include <string>
#include "syntax.tab.h"

%}
%%

[ \r\t]        {}
\n             { return NEWLINE; }
"-->"          { return ARROW; }
"="            { return ASSIGN; }
"K"            { return K; }
"vr"           { return VR; }
[0-9]+         { yylval.integer = atof(yytext); return INTEGER; }
.              { std::cout << "Invalid Symbol: " << yytext << std::endl; }

%%