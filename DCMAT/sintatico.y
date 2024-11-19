%code requires {
// Necessário para evitar problemas de inclusão
#include "utils.hpp"
#include <iostream>
#include <cmath>
}

%{
    extern int yylex();
%}

// ARRUMAR TIPOS E UNION
%union {
    int int_val;
    double double_val;
}

%token <double_val> INTEGER
%token <double_val> REAL_NUMBER
%type <double_val> Expression
%type <double_val> Number

%token PLUS MINUS MULTIPLY DIVIDE EXPONENT MODULO LEFT_PAREN RIGHT_PAREN SIN COS TAN ABS VARIABLE IDENTIFIER 
PI_CONSTANT EULER_CONSTANT ABOUT FLOAT SETTINGS H_VIEW PLOT SHOW AXIS INTEGRAL_STEPS PRECISION SOLVE 
CONNECT_DOTS INTEGRATE QUIT SUM LINEAR_SYSTEM RESET SYMBOLS DETERMINANT MATRIX RPN OFF V_VIEW ERASE ON SET COLON EQUAL ASSIGN 
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
    | Expression { printf("%lf\n", $1);}
    |
    ;

Command:
       | SHOW SETTINGS
       | RESET SETTINGS
       | SET H_VIEW REAL_NUMBER COLON REAL_NUMBER
       | SET V_VIEW REAL_NUMBER COLON REAL_NUMBER
       | SET AXIS ON
       | SET AXIS OFF
       | PLOT
       | PLOT LEFT_PAREN Function RIGHT_PAREN
       | SET ERASE PLOT OFF
       | SET ERASE PLOT ON
       | RPN LEFT_PAREN Expression RIGHT_PAREN
       | SET INTEGRAL_STEPS INTEGER
       | INTEGRATE LEFT_PAREN Interval COMMA Function RIGHT_PAREN
       | SUM LEFT_PAREN IDENTIFIER COMMA Interval COMMA Expression RIGHT_PAREN 
       | MatrixCreate
       | SHOW MATRIX
       | SOLVE DETERMINANT
       | SOLVE LINEAR_SYSTEM
       | ABOUT
       | IDENTIFIER ASSIGN Expression
       | IDENTIFIER ASSIGN MatrixCreate
       | IDENTIFIER
       | SHOW SYMBOLS
       | SET FLOAT PRECISION INTEGER
       | SET CONNECT_DOTS ON
       | SET CONNECT_DOTS OFF
       ;

Function: Expression
        ;

MatrixCreate: 
            MATRIX EQUAL LEFT_BRACKET LEFT_BRACKET Number MatrixNumberLoop RIGHT_BRACKET MatrixCreateLoop RIGHT_BRACKET
            ;

MatrixNumberLoop:
                COMMA Number MatrixNumberLoop
                |
                ;

MatrixCreateLoop:
            COMMA LEFT_BRACKET Number MatrixNumberLoop RIGHT_BRACKET MatrixCreateLoop
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
    | PLUS Expression { $$ = +$2; }
    | MINUS Expression { $$ = -$2; }
    | Number { $$ = $1; }
    ;

Number: INTEGER
      | REAL_NUMBER
      ;

Interval: Number COLON Number
%%
