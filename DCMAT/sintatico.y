%code requires {
// Necessário para evitar problemas de inclusão
// Coloca na .h
#include "utils.hpp"
#include "dcmat.hpp"
#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>
}

%{
    // Coloca na .c
    extern int yylex();
%}

%union {
    double value;
    std::string* str;
    std::pair<int, int>* int_interval;
    std::pair<double, double>* interval;
    std::vector<double>* vector;
    Function *function;
    Matrix *matrix;
}

%token <str> IDENTIFIER
%token <value> INTEGER REAL_NUMBER
%type <value> Expression Number Integer
%type <interval> Interval
%type <int_interval> IntegerInterval
%type <function> Function FunctionExpression
%type <matrix> MatrixCreate MatrixCreateLoop
%type <vector> MatrixRow MatrixNumberLoop 

%token PLUS MINUS MULTIPLY DIVIDE EXPONENT MODULO LEFT_PAREN RIGHT_PAREN SIN COS TAN ABS X
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
       SHOW SETTINGS SEMICOLON { dcmat.settings.show(); }
       | RESET SETTINGS SEMICOLON { dcmat.settings.reset(); }
       | SET H_VIEW Interval SEMICOLON { dcmat.settings.setHView(*$3); delete $3; }
       | SET V_VIEW Interval SEMICOLON { dcmat.settings.setVView(*$3); delete $3; }
       | SET AXIS ON SEMICOLON { dcmat.settings.draw_axis = true; }
       | SET AXIS OFF SEMICOLON { dcmat.settings.draw_axis = false; }
       | PLOT SEMICOLON { dcmat.plot(); }
       | PLOT LEFT_PAREN Function RIGHT_PAREN SEMICOLON {
            dcmat.setLastFunction(std::unique_ptr<Function>($3));
            dcmat.plot();
        }
       | SET ERASE PLOT OFF SEMICOLON { dcmat.settings.erase_plot = false; }
       | SET ERASE PLOT ON SEMICOLON { dcmat.settings.erase_plot = true; }
       | RPN LEFT_PAREN FunctionExpression RIGHT_PAREN SEMICOLON {
            std::cout << "Expression in RPN format: " << std::endl << $3->toRPN() << std::endl;
            delete $3;
        }
       | SET INTEGRAL_STEPS INTEGER SEMICOLON { dcmat.settings.integral_steps = $3; }
       | INTEGRATE LEFT_PAREN Interval COMMA Function RIGHT_PAREN SEMICOLON {
            std::cout << dcmat.integrate(*$3, *$5) << std::endl;
            delete $3;
            delete $5;
        }
       | SUM LEFT_PAREN IDENTIFIER COMMA IntegerInterval COMMA FunctionExpression RIGHT_PAREN SEMICOLON {
            std::cout << dcmat.sum(*$3, *$5, *$7) << std::endl;
            delete $3;
            delete $5;
            delete $7;
        }
       | MatrixCreate SEMICOLON { 
            dcmat.setLastMatrix(std::unique_ptr<Matrix>($1));
        }
       | SHOW MATRIX SEMICOLON {
            try {
              dcmat.getLastMatrix().printMatrix();
            } catch (const std::runtime_error &e) {
              std::cout << e.what() << std::endl;
            }
        }
       | SOLVE DETERMINANT SEMICOLON {
            try {
              std::cout << dcmat.getLastMatrix().determinant() << std::endl;
            } catch (const std::runtime_error &e) {
              std::cout << e.what() << std::endl;
            }
        }
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

Function: FunctionExpression { $$ = $1; }
        ;

FunctionExpression:
                  FunctionExpression PLUS FunctionExpression { 
                    $$ = new Function(
                        [](double a, double b) { return a + b; },
                        std::unique_ptr<Function>($1),
                        std::unique_ptr<Function>($3),
                        "+"
                    );
                  }
                  | FunctionExpression MINUS FunctionExpression { 
                    $$ = new Function(
                        [](double a, double b) { return a - b; },
                        std::unique_ptr<Function>($1),
                        std::unique_ptr<Function>($3),
                        "-"
                    );
                  }
                  | FunctionExpression MULTIPLY FunctionExpression { 
                    $$ = new Function(
                        [](double a, double b) { return a * b; },
                        std::unique_ptr<Function>($1),
                        std::unique_ptr<Function>($3),
                        "*"
                    );
                  }
                  | FunctionExpression DIVIDE FunctionExpression { 
                    $$ = new Function(
                        [](double a, double b) { return a / b; },
                        std::unique_ptr<Function>($1),
                        std::unique_ptr<Function>($3),
                        "/"
                    );
                  }
                  | FunctionExpression MODULO FunctionExpression { 
                    $$ = new Function(
                        [](double a, double b) { return std::fmod(a, b); },
                        std::unique_ptr<Function>($1),
                        std::unique_ptr<Function>($3),
                        "%"
                    );
                  }
                  | FunctionExpression EXPONENT FunctionExpression { 
                    $$ = new Function(
                        [](double a, double b) { return std::pow(a, b); },
                        std::unique_ptr<Function>($1),
                        std::unique_ptr<Function>($3),
                        "^"
                    );
                  }
                  | LEFT_PAREN FunctionExpression RIGHT_PAREN { 
                    $$ = new Function(
                        [](double x) { return x; },
                        std::unique_ptr<Function>($2),
                        "\b"
                    );
                  }
                  | SIN LEFT_PAREN FunctionExpression RIGHT_PAREN { 
                    $$ = new Function(
                        [](double x) { return std::sin(x); },
                        std::unique_ptr<Function>($3),
                        "SEN"
                    );
                  }
                  | COS LEFT_PAREN FunctionExpression RIGHT_PAREN { 
                    $$ = new Function(
                        [](double x) { return std::cos(x); },
                        std::unique_ptr<Function>($3),
                        "COS"
                    );
                  }
                  | TAN LEFT_PAREN FunctionExpression RIGHT_PAREN { 
                    $$ = new Function(
                        [](double x) { return std::tan(x); },
                        std::unique_ptr<Function>($3),
                        "TAN"
                    );
                  }
                  | ABS LEFT_PAREN FunctionExpression RIGHT_PAREN { 
                    $$ = new Function(
                        [](double x) { return std::abs(x); },
                        std::unique_ptr<Function>($3),
                        "ABS"
                    );
                  }
                  | PI_CONSTANT {
                    $$ = new Function(
                        [](double) { return M_PI; },
                        std::to_string(M_PI)
                    );
                  }
                  | EULER_CONSTANT {
                    $$ = new Function(
                        [](double) { return M_E; },
                        std::to_string(M_E)
                    );
                  }
                  | PLUS FunctionExpression {
                    $$ = new Function(
                        [](double x) { return +x; },
                        std::unique_ptr<Function>($2),
                        "+"
                    );
                  }
                  | MINUS FunctionExpression { 
                    $$ = new Function(
                        [](double x) { return -x; },
                        std::unique_ptr<Function>($2),
                        "-"
                    );
                  }
                  | INTEGER { 
                    $$ = new Function(
                        [=](double) { return $1; },
                        std::to_string($1)
                    );
                  }
                  | REAL_NUMBER { 
                    $$ = new Function(
                        [=](double) { return $1; },
                        std::to_string($1)
                    );
                  }
                  | X { 
                    $$ = new Function(
                        [](double x) { return x; },
                        "x"
                    );
                  }
                  | IDENTIFIER {
                    $$ = new Function(
                      [identifier = std::string(*$1)](double) {
                          return dcmat.getVariable(identifier); 
                      },
                      *$1
                    );
                    delete $1;
                  }
                  ;

MatrixCreate: 
            MATRIX EQUAL LEFT_BRACKET MatrixRow MatrixCreateLoop RIGHT_BRACKET {
              (*$5) += (*$4);
              $5->reverse();
              $$ = $5;
              delete $4;
            }
            ;

MatrixCreateLoop:
            COMMA MatrixRow MatrixCreateLoop {
                (*$3) += (*$2);
                $$ = $3;
                delete $2;
            }
            | { $$ = new Matrix(); }
            ;
          
MatrixRow:
          LEFT_BRACKET Number MatrixNumberLoop RIGHT_BRACKET {
              $3->push_back($2);
              std::reverse( $3->begin(),  $3->end()); // Coloca na ordem correta
              $$ = $3;
          }
          ;

MatrixNumberLoop:
                COMMA Number MatrixNumberLoop { $3->push_back($2); $$ = $3; }
                | { $$ = new std::vector<double> {}; }
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

Integer: INTEGER { $$ = $1; }
      | MINUS INTEGER { $$ = -$2; }
      | PLUS INTEGER { $$ = +$2; }
      ;

Interval: Number COLON Number { $$ = new std::pair<double, double>($1, $3); }
        ;

IntegerInterval: Integer COLON Integer { $$ = new std::pair<int, int>($1, $3); }
        ;

%%