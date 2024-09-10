%{
#include <iostream>
#include "utils.hpp"
void yyerror(const char *s);
extern int yylex();
%}

%token NUMBER REAL_NUMBER VARIABLE PLUS MINUS MULTIPLY DIVIDE POW REST L_PAREN R_PAREN SEN COS TAN ABS

%start expr

%%

operator: PLUS | MINUS | ;
var: NUMBER | VARIABLE | REAL_NUMBER;
opvar: operator var
expr: L_PAREN expr R_PAREN 
    | expr PLUS expr 
    | expr MINUS expr 
    | expr MULTIPLY expr 
    | expr DIVIDE expr
    | expr REST expr
    | opvar POW opvar
    | L_PAREN expr R_PAREN POW opvar
    | SEN L_PAREN expr R_PAREN 
    | COS L_PAREN expr R_PAREN 
    | TAN L_PAREN expr R_PAREN 
    | ABS L_PAREN expr R_PAREN 
    | opvar

%%

void yyerror(const char *s) {
    log_error(s);
}
