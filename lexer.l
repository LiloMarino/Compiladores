%{
#include "parser.tab.h"
#include "utils.hpp"
#include <iostream>
int column = 1;
%}

%%

[ \t]+          {/* Ignora */}
"sen"           { column += yyleng; return SEN; }
"cos"           { column += yyleng; return COS; }
"tan"           { column += yyleng; return TAN; }
"abs"           { column += yyleng; return ABS; }
[0-9]+          { column += yyleng; return NUMBER; }
[0-9]*"."[0-9]+ { column += yyleng; return REAL_NUMBER; }
[0-9]+"."[0-9]* { column += yyleng; return REAL_NUMBER; }
[a-z][a-z0-9]+  { column += yyleng; return VARIABLE; }
"+"             { column += yyleng; return PLUS; }
"-"             { column += yyleng; return MINUS; }
"*"             { column += yyleng; return MULTIPLY; }
"/"             { column += yyleng; return DIVIDE; }
"^"             { column += yyleng; return POW; }
"%"             { column += yyleng; return REST; }
"("             { column += yyleng; return L_PAREN; }
")"             { column += yyleng; return R_PAREN; }
\n              { column = 1; }
.               { printf("ERROR:  %s\n", yytext);}

%%

int yywrap() {
    return 1;
}
