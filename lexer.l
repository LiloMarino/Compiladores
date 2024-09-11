%{
#include "parser.tab.h"
#include <iostream>
#include <cstring>
#include <cstdlib>
%}

%%
"int"          { return INT; }
"float"        { return FLOAT; }
"char"         { return CHAR; }
[a-z]+         { yylval.str = strdup(yytext); return ID; }
[ \t\n\r]+     { /* Ignore whitespace */ }
","            { return ','; }
"$"            { return '$'; }
.              { std::cerr << "Invalid character: " << yytext << std::endl; exit(1); }
%%

int yywrap() {
    return 1;
}
