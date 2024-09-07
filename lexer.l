%{
#include "parser.tab.h"
#include "utils.hpp"
%}

%%

[ \t\n]+       ;
[0-9]+         { return NUMBER; }
"+"            { return PLUS; }
"-"            { return MINUS; }
"*"            { return MULTIPLY; }
"/"            { return DIVIDE; }
.              { 
                 log_error("Token não reconhecido");
                 return yytext[0]; 
               }

%%

int yywrap() {
    return 1;
}
