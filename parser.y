%{
#include <iostream>
#include "utils.hpp"
void yyerror(const char *s);
extern int yylex();
%}

%token NUMBER PLUS MINUS MULTIPLY DIVIDE

%%

input:
    | input expr '\n' { std::cout << "Resultado: " << $2 << std::endl; }
    ;

expr:
    NUMBER                { $$ = $1; }
    | expr PLUS expr      { $$ = $1 + $3; }
    | expr MINUS expr     { $$ = $1 - $3; }
    | expr MULTIPLY expr  { $$ = $1 * $3; }
    | expr DIVIDE expr    { $$ = $1 / $3; }
    ;

%%

void yyerror(const char *s) {
    log_error(s);
}
