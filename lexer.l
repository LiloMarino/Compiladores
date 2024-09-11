%{
#include "parser.tab.h"
#include "utils.hpp"
#include <iostream>
#include <vector>
int column = 1;
bool lexical_error = false;
std::vector<std::string> invalid_tokens;
%}

%%

[ \t]+          { column += yyleng; /* Ignora espaços e tabulações */ }
"sen"           { column += yyleng; return SEN; }
"cos"           { column += yyleng; return COS; }
"tan"           { column += yyleng; return TAN; }
"abs"           { column += yyleng; return ABS; }
[0-9]+\.[0-9]*  { column += yyleng; return REAL_NUMBER; }
[0-9]*\.[0-9]+  { column += yyleng; return REAL_NUMBER; }
[0-9]+          { column += yyleng; return NUMBER; }
[a-z][a-z0-9]*  { column += yyleng; return VARIABLE; }
"+"             { column += yyleng; return PLUS; }
"-"             { column += yyleng; return MINUS; }
"*"             { column += yyleng; return MULTIPLY; }
"/"             { column += yyleng; return DIVIDE; }
"^"             { column += yyleng; return POW; }
"%"             { column += yyleng; return REST; }
"("             { column += yyleng; return L_PAREN; }
")"             { column += yyleng; return R_PAREN; }
\n              { column = 1; if (lexical_error) { 
                                log_lexical_error();
                                lexical_error = false; 
                            } 
                            return '\n'; }
<<EOF>>         {exit(0);}
.               { lexical_error = true; 
                invalid_tokens.push_back(yytext); 
                column += yyleng; 
                /* Continuamos para capturar todos os caracteres inválidos */ }

%%

int yywrap() {
    return 1;
}
