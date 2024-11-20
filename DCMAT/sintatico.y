%code requires {
// Necessário para evitar problemas de inclusão
// Coloca na .h
#include "utils.hpp"
#include "dcmat.hpp"
#include <iostream>
#include <cmath>
}

%{
    // Coloca na .c
    extern int yylex();
%}

%union {
    double value;
    std::pair<double, double>* interval;
    std::function<double(double)> *function;
}


%token <value> INTEGER REAL_NUMBER
%type <value> Expression Number
%type <interval> Interval
%type <function> Function FunctionExpression

%token PLUS MINUS MULTIPLY DIVIDE EXPONENT MODULO LEFT_PAREN RIGHT_PAREN SIN COS TAN ABS X IDENTIFIER 
PI_CONSTANT EULER_CONSTANT ABOUT FLOAT SETTINGS H_VIEW PLOT SHOW AXIS INTEGRAL_STEPS PRECISION SOLVE 
CONNECT_DOTS INTEGRATE QUIT SUM LINEAR_SYSTEM RESET SYMBOLS DETERMINANT MATRIX RPN OFF V_VIEW ERASE ON SET COLON 
EQUAL ASSIGN LEFT_BRACKET RIGHT_BRACKET SEMICOLON COMMA

// Definindo precedência e associatividade
%left PLUS MINUS
%left MULTIPLY DIVIDE MODULO
%right EXPONENT
%left LEFT_PAREN RIGHT_PAREN

%start Program

%%

Program:
    Command
    | QUIT { exit(0); }
    | Expression { printf("%lf\n", $1);}
    |
    ;

Command:
       SHOW SETTINGS SEMICOLON { settings.show(); }
       | RESET SETTINGS SEMICOLON { settings.reset(); }
       | SET H_VIEW Interval SEMICOLON { settings.setHView(*$3); delete $3; }
       | SET V_VIEW Interval SEMICOLON { settings.setVView(*$3); delete $3; }
       | SET AXIS ON SEMICOLON { settings.draw_axis = true; }
       | SET AXIS OFF SEMICOLON { settings.draw_axis = false; }
       | PLOT SEMICOLON {        
            if (last_function) {
                plot(last_function);
            } else {
                std::cout << "No function defined!" << std::endl;
            }
        }
       | PLOT LEFT_PAREN Function RIGHT_PAREN SEMICOLON {
            last_function = $3;
            plot(last_function);
        }
       | SET ERASE PLOT OFF SEMICOLON
       | SET ERASE PLOT ON SEMICOLON
       | RPN LEFT_PAREN Expression RIGHT_PAREN SEMICOLON
       | SET INTEGRAL_STEPS INTEGER SEMICOLON
       | INTEGRATE LEFT_PAREN Interval COMMA Function RIGHT_PAREN SEMICOLON
       | SUM LEFT_PAREN IDENTIFIER COMMA Interval COMMA Expression RIGHT_PAREN SEMICOLON
       | MatrixCreate SEMICOLON
       | SHOW MATRIX SEMICOLON
       | SOLVE DETERMINANT SEMICOLON
       | SOLVE LINEAR_SYSTEM SEMICOLON
       | ABOUT SEMICOLON
       | IDENTIFIER ASSIGN Expression SEMICOLON
       | IDENTIFIER ASSIGN MatrixCreate SEMICOLON
       | IDENTIFIER SEMICOLON
       | SHOW SYMBOLS SEMICOLON
       | SET FLOAT PRECISION INTEGER SEMICOLON
       | SET CONNECT_DOTS ON SEMICOLON
       | SET CONNECT_DOTS OFF SEMICOLON
       ;

Function: FunctionExpression { $$ = $1; };
        ;

FunctionExpression: 
                  FunctionExpression PLUS FunctionExpression   { $$ = $1 + $3; }
                  | FunctionExpression MINUS FunctionExpression  { $$ = $1 - $3; }
                  | FunctionExpression MULTIPLY FunctionExpression { $$ = $1 * $3; }
                  | FunctionExpression DIVIDE FunctionExpression   { $$ = $1 / $3; }
                  | FunctionExpression MODULO FunctionExpression { $$ = std::fmod($1, $3); }
                  | FunctionExpression EXPONENT FunctionExpression { $$ = std::pow($1, $3); }
                  | LEFT_PAREN FunctionExpression RIGHT_PAREN { $$ = $2; }
                  | SIN LEFT_PAREN FunctionExpression RIGHT_PAREN { $$ = std::sin($3); }
                  | COS LEFT_PAREN FunctionExpression RIGHT_PAREN { $$ = std::cos($3); }
                  | TAN LEFT_PAREN FunctionExpression RIGHT_PAREN { $$ = std::tan($3); }
                  | ABS LEFT_PAREN FunctionExpression RIGHT_PAREN { $$ = std::abs($3); }
                  | PI_CONSTANT { $$ = M_PI; }
                  | EULER_CONSTANT { $$ = M_E; }
                  | PLUS FunctionExpression { $$ = +$2; }
                  | MINUS FunctionExpression { $$ = -$2; }
                  | INTEGER { $$ = $1; }
                  | REAL_NUMBER { $$ = $1; }
                  | X { $$ = new std::function<double(double)>([](double x) { return x; }); }
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
    | Expression DIVIDE Expression   { $$ = $1 / $3; }
    | Expression MODULO Expression { $$ = std::fmod($1, $3); }
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
    | INTEGER { $$ = $1; }
    | REAL_NUMBER { $$ = $1; }
    ;

Number:          
      REAL_NUMBER { $$ = $1;  }
      | MINUS REAL_NUMBER { $$ = -$2; }
      | PLUS REAL_NUMBER { $$ = +$2; }
      | INTEGER { $$ = $1; }
      | MINUS INTEGER { $$ = -$2; }
      | PLUS INTEGER { $$ = +$2; }
      ;

Interval: Number COLON Number { $$ = new std::pair<double, double>($1, $3); }
        ;

%%