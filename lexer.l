%{
#include <string>
#include "parser.tab.h"
%}

%%

"int"          { return INT; }
"float"        { return FLOAT; }
"char"         { return CHAR; }
[a-z]+         { yylval.str = new std::string(yytext); return ID; }
","            { return ','; }
"$"            { return '$'; }
[ \t\r]+       { /* Ignora espaços, tabulações e retornos de carro */ }
\n             { yylineno++; }
.              { /* Ignora qualquer outro caractere */ }

%%

int yywrap() {
    return 1;
}
