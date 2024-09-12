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
.              { /* Ignora qualquer outro caractere */ }
[ \t\n]+       { /* Ignora espaços e novas linhas */ }

%%

int yywrap() {
    return 1;
}
