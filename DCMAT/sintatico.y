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
%type <function> FunctionExpression
%type <matrix> MatrixCreate MatrixCreateLoop
%type <vector> MatrixRow MatrixNumberLoop
%type <str> SumVariable

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
    | Expression {
        if (dcmat.isValidExpression())
        {
          std::cout << $1 << std::endl;
        }
      }
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
       | PLOT LEFT_PAREN FunctionExpression RIGHT_PAREN SEMICOLON {
            if (dcmat.isValidExpression()) {
              dcmat.setLastFunction(std::unique_ptr<Function>($3));
              dcmat.plot();
            }
            else {
              delete $3;
            }
        }
       | SET ERASE PLOT OFF SEMICOLON { dcmat.settings.erase_plot = false; }
       | SET ERASE PLOT ON SEMICOLON { dcmat.settings.erase_plot = true; }
       | RPN LEFT_PAREN FunctionExpression RIGHT_PAREN SEMICOLON {
            std::cout << "Expression in RPN format: " << std::endl << $3->toRPN() << std::endl;
            delete $3;
        }
       | SET INTEGRAL_STEPS INTEGER SEMICOLON { dcmat.settings.integral_steps = $3; }
       | INTEGRATE LEFT_PAREN Interval COMMA FunctionExpression RIGHT_PAREN SEMICOLON {
            if (dcmat.isValidExpression()) {
              std::cout << dcmat.integrate(*$3, *$5) << std::endl;
            }
            delete $3;
            delete $5;
        }
       | SUM LEFT_PAREN SumVariable COMMA IntegerInterval COMMA FunctionExpression RIGHT_PAREN SEMICOLON {
            if (dcmat.isValidExpression()) {
              std::cout << dcmat.sum(*$3, *$5, *$7) << std::endl;
            }
            delete $3;
            delete $5;
            delete $7;
        }
       | MATRIX EQUAL MatrixCreate SEMICOLON { 
            dcmat.setLastMatrix(std::unique_ptr<Matrix>($3));
        }
       | SHOW MATRIX SEMICOLON {
            try {
              dcmat.getLastMatrix().printMatrix();
            } 
            catch (const std::runtime_error &e) {
              std::cout << e.what() << std::endl;
            }
        }
       | SOLVE DETERMINANT SEMICOLON {
            try {
              std::cout << dcmat.getLastMatrix().determinant() << std::endl;
            } 
            catch (const std::runtime_error &e) {
              std::cout << e.what() << std::endl;
            }
        }
       | SOLVE LINEAR_SYSTEM SEMICOLON {
            try {
              std::vector<double> solution = dcmat.getLastMatrix().solveLinearSystem();
              std::cout << std::endl << "Matrix x:" << std::endl;
              for (double value : solution) {
                std::cout << value << std::endl;
              }
            } 
            catch (const std::runtime_error &e) {
              std::cout << std::endl << e.what() << std::endl;
            }
            std::cout << std::endl;
        }
       | ABOUT SEMICOLON { dcmat.about(); }
       | IDENTIFIER ASSIGN Expression SEMICOLON {
          if (dcmat.isValidExpression()) {
            dcmat.setVariable(*$1, $3);
            std::cout << dcmat.getVariable(*$1) << std::endl;
          }
          delete $1;
        }
       | IDENTIFIER ASSIGN MatrixCreate SEMICOLON {
          dcmat.setVariable(*$1, std::move(*$3));
          std::cout << dcmat.getVariable(*$1) << std::endl;
          delete $1;
        }
       | IDENTIFIER SEMICOLON {
          std::cout << dcmat.getVariable(*$1) << std::endl;
          delete $1;
        } 
       | SHOW SYMBOLS SEMICOLON
       | SET FLOAT PRECISION INTEGER SEMICOLON
       | SET CONNECT_DOTS ON SEMICOLON
       | SET CONNECT_DOTS OFF SEMICOLON
       ;

SumVariable: IDENTIFIER { $$ = $1; dcmat.setVariable(*$1, 0); }

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
                    // Testa para ver se o símbolo existe
                    dcmat.getVariable(*$1);
                    $$ = new Function(
                      [identifier = std::string(*$1)](double) {
                          return dcmat.getVariable(identifier).getNumber(); 
                      },
                      *$1
                    );
                    delete $1;
                  }
                  ;

MatrixCreate: 
            LEFT_BRACKET MatrixRow MatrixCreateLoop RIGHT_BRACKET {
              (*$3) += (*$2);
              $3->reverse();
              $$ = $3;
              delete $2;
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
    | IDENTIFIER {
        DynamicTyping &var = dcmat.getVariable(*$1);
        if (dcmat.isValidExpression(false))
        {
            $$ = var.isNumber() ? var.getNumber() : 0; // Pega número se existir
        }
        delete $1;
    }
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