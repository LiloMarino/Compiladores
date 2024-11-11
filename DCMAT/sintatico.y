%code requires {
// Necessário para evitar problemas de inclusão
#include "utils.hpp"
#include <iostream>
}

%{
    #include <cstdio>
    #include <cmath>
    #include <cstdlib>
    extern int yylex();
    void yyerror(const char *s);
%}

%union {
    int int_val;
    double double_val;
}

%token <double_val> INTEGER
%token <double_val> REAL_NUMBER
%type <double_val> Expression

%token PLUS MINUS MULTIPLY DIVIDE EXPONENT MODULO LEFT_PAREN RIGHT_PAREN SIN COS TAN ABS VARIABLE IDENTIFIER 
PI_CONSTANT EULER_CONSTANT ABOUT FLOAT SETTINGS H VIEW PLOT SHOW AXIS INTEGRAL STEPS PRECISION SOLVE CONNECT 
DOTS INTEGRATE QUIT SUM LINEAR SYSTEM RESET SYMBOLS DETERMINANT MATRIX RPN OFF V ERASE ON SET COLON EQUAL ASSIGN 
LEFT_BRACKET RIGHT_BRACKET SEMICOLON COMMA


%start Program

%%

Program:
    Expression { printf("Resultado: %lf\n", $1); }
    ;

Expression:
    Expression PLUS Expression   { $$ = $1 + $3; }
    | Expression MINUS Expression  { $$ = $1 - $3; }
    | Expression MULTIPLY Expression { $$ = $1 * $3; }
    | Expression DIVIDE Expression   {
        if ($3 == 0) {
            yyerror("Erro: divisão por zero");
            $$ = 0;
        } else {
            $$ = $1 / $3;
        }
    }
    | Expression EXPONENT Expression { $$ = pow($1, $3); }
    | LEFT_PAREN Expression RIGHT_PAREN { $$ = $2; }
    | INTEGER { $$ = $1; std::cout << $1 << std::endl;}
    | REAL_NUMBER { $$ = $1; std::cout << $1 << std::endl; }
    ;

%%
