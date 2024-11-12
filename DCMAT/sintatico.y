%code requires {
// Necessário para evitar problemas de inclusão
#include "utils.hpp"
#include <iostream>
#include <cmath>
}

%{
    extern int yylex();
%}

%union {
    int int_val;
    double double_val;
}

%token <double_val> INTEGER
%token <double_val> REAL_NUMBER
%type <double_val> Expression

%token PLUS MINUS MULTIPLY DIVIDE EXPONENT MODULO LEFT_PAREN RIGHT_PAREN SIN COS TAN ABS VARIABLE IDENTIFIER 
PI_CONSTANT EULER_CONSTANT ABOUT FLOAT SETTINGS H_VIEW PLOT SHOW AXIS INTEGRAL STEPS PRECISION SOLVE CONNECT 
DOTS INTEGRATE QUIT SUM LINEAR SYSTEM RESET SYMBOLS DETERMINANT MATRIX RPN OFF V_VIEW ERASE ON SET COLON EQUAL ASSIGN 
LEFT_BRACKET RIGHT_BRACKET SEMICOLON COMMA

// Definindo precedência e associatividade
%left PLUS MINUS
%left MULTIPLY DIVIDE MODULO
%right EXPONENT
%left LEFT_PAREN RIGHT_PAREN

%start Program

%%

Program:
    Command SEMICOLON
    | QUIT { exit(0); }
    |
    ;

Command:
       Expression { printf("Resultado: %lf\n", $1);} // REMOVER
       | SHOW SETTINGS
       | RESET SETTINGS
       | SET 
       | 
       | 
       | 
       | 
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
    | Expression MODULO Expression {
        if ($3 == 0) {
            yyerror("Erro: módulo por zero");
            $$ = 0;
        } else {
            $$ = std::fmod($1, $3);
        }
    }
    | Expression EXPONENT Expression { $$ = std::pow($1, $3); }
    | LEFT_PAREN Expression RIGHT_PAREN { $$ = $2; }
    | SIN LEFT_PAREN Expression RIGHT_PAREN { $$ = std::sin($3); }
    | COS LEFT_PAREN Expression RIGHT_PAREN { $$ = std::cos($3); }
    | TAN LEFT_PAREN Expression RIGHT_PAREN { $$ = std::tan($3); }
    | ABS LEFT_PAREN Expression RIGHT_PAREN { $$ = std::abs($3); }
    | PI_CONSTANT { $$ = M_PI; }
    | EULER_CONSTANT { $$ = M_E; }
    | MINUS INTEGER { $$ = -$2; }
    | PLUS INTEGER { $$ = +$2; }
    | INTEGER { $$ = $1; }
    | MINUS REAL_NUMBER { $$ = -$2; }
    | PLUS REAL_NUMBER { $$ = +$2; }
    | REAL_NUMBER { $$ = $1; }
    ;

%%
