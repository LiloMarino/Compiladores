%code requires {
// Necessário para evitar problemas de inclusão
// Coloca na .h
#include <iostream>
#include <memory>
#include <deque>
#include "expression.hpp"
#include "type.hpp"
#include "command.hpp"
#include "variable.hpp"
#include "function.hpp"
#include "ast.hpp"
}

%{
// Coloca na .c
extern int yylex();
void yyerror(const char *msg);
%}
%debug
%verbose
%union {
    int integer;
    std::string* str;
    Type* type;
    Expression* exp;
    Command* cmd;
    Variable* var;
    Function* func;
    OperatorType op;
    std::deque<int>*  deque_int;
    std::deque<std::unique_ptr<Expression>>*  deque_exp;
    std::deque<std::unique_ptr<Command>>*  deque_cmd;
    std::deque<std::unique_ptr<Variable>>*  deque_var;
}

%token <str> IDENTIFIER STRING CHARACTER
%token <integer> INTEGER
%type <op> Operator
%type <integer> Integer StarLoop
%type <deque_int> Dimension Dimensions
%type <type> Type ReturnType
%type <exp> Expression TernaryExpression BinaryExpression UnaryExpression Condition Assign
%type <deque_exp> Expressions
%type <cmd> Command
%type <deque_cmd> Commands
%type <var> Constant LocalVariable GlobalVariable Parameter
%type <deque_var> Variables Parameters
%type <func> Function

%token GLOBAL VARIABLE CONSTANT PARAMETER VALUE RETURN_TYPE TYPE VOID INT CHAR FUNCTION END_FUNCTION RETURN DO_WHILE
WHILE FOR IF PRINTF SCANF EXIT PLUS MINUS MULTIPLY DIVIDE REMAINDER INC DEC BITWISE_AND BITWISE_OR BITWISE_NOT
BITWISE_XOR NOT LOGICAL_AND LOGICAL_OR EQUAL NOT_EQUAL LESS_THAN GREATER_THAN LESS_EQUAL GREATER_EQUAL R_SHIFT L_SHIFT
ASSIGN ADD_ASSIGN MINUS_ASSIGN SEMICOLON COMMA COLON L_PAREN R_PAREN L_SQUARE_BRACKET R_SQUARE_BRACKET
TERNARY_OPERATOR EOF_TOKEN

%start AstStart

%%

AstStart: AstLoop EOF_TOKEN { Ast::translate(); }
        ;

AstLoop: Declaration AstLoop
       |
       ;

Declaration: Constant { Ast::addVariable(std::unique_ptr<Variable>($1)); }
           | GlobalVariable { Ast::addVariable(std::unique_ptr<Variable>($1)); }
           | Function { Ast::addFunction(std::unique_ptr<Function>($1)); }
           ;

Constant: CONSTANT COLON IDENTIFIER VALUE COLON Integer {
          $$ = new Variable(*$3, $6);
         }
        ;

GlobalVariable: GLOBAL VARIABLE COLON IDENTIFIER TYPE COLON Type {
                $$ = new Variable(VariableCategory::GLOBAL_VARIABLE, std::unique_ptr<Type>($7), *$4);
               }
              ;

Function: FUNCTION COLON IDENTIFIER RETURN_TYPE COLON ReturnType Parameters Variables Commands END_FUNCTION {
            $$ = new Function(*$3, std::unique_ptr<Type>($6), 
                              std::unique_ptr<std::deque<std::unique_ptr<Variable>>>($7), 
                              std::unique_ptr<std::deque<std::unique_ptr<Variable>>>($8), 
                              std::unique_ptr<std::deque<std::unique_ptr<Command>>>($9));
         }
        ;

Parameters: Parameter Parameters {
            (*$2).emplace_front(std::move($1));
            $$ = $2;
           }
          | { 
            $$ = new std::deque<std::unique_ptr<Variable>>();
           }
          ;

Parameter: PARAMETER COLON IDENTIFIER TYPE COLON Type {
            $$ = new Variable(VariableCategory::PARAMETER, std::unique_ptr<Type>($6), *$3);
          }
         ;

Variables: LocalVariable Variables {
            (*$2).emplace_front(std::move($1));
            $$ = $2;
          }
         | { 
            $$ = new std::deque<std::unique_ptr<Variable>>();
          }
         ;

LocalVariable: VARIABLE COLON IDENTIFIER TYPE COLON Type {
                $$ = new Variable(VariableCategory::LOCAL_VARIABLE, std::unique_ptr<Type>($6), *$3); 
              }
             ;

Commands: Command SEMICOLON Commands {
            (*$3).emplace_front(std::move($1));
            $$ = $3;
         }
        | Command {
            $$ = new std::deque<std::unique_ptr<Command>>();
            (*$$).emplace_front(std::move($1));
         }
        ;

Command: DO_WHILE L_PAREN Commands COMMA Condition R_PAREN {
          $$ = new Command(CommandType::DO_WHILE, std::unique_ptr<Expression>($5), 
                           std::unique_ptr<std::deque<std::unique_ptr<Command>>>($3));
        }
       | IF L_PAREN Condition COMMA Commands COMMA Commands R_PAREN {
          $$ = new Command(std::unique_ptr<Expression>($3),
                           std::unique_ptr<std::deque<std::unique_ptr<Command>>>($5),
                           std::unique_ptr<std::deque<std::unique_ptr<Command>>>($7));
        }
       | IF L_PAREN Condition COMMA Commands R_PAREN {
          $$ = new Command(CommandType::IF, std::unique_ptr<Expression>($3), 
                           std::unique_ptr<std::deque<std::unique_ptr<Command>>>($5));
        }
       | WHILE L_PAREN Condition COMMA Commands R_PAREN {
          $$ = new Command(CommandType::WHILE, std::unique_ptr<Expression>($3), 
                           std::unique_ptr<std::deque<std::unique_ptr<Command>>>($5));
        }
       | FOR L_PAREN Assign COMMA Condition COMMA Assign COMMA Commands R_PAREN {
          $$ = new Command(std::unique_ptr<Expression>($3), std::unique_ptr<Expression>($5),
                           std::unique_ptr<Expression>($7), std::unique_ptr<std::deque<std::unique_ptr<Command>>>($9));
        }
       | PRINTF L_PAREN STRING COMMA Expressions R_PAREN {
          $$ = new Command(*$3, std::unique_ptr<std::deque<std::unique_ptr<Expression>>>($5));
        }
       | PRINTF L_PAREN STRING R_PAREN {
          $$ = new Command(*$3);
        }
       | SCANF L_PAREN STRING COMMA BITWISE_AND L_PAREN IDENTIFIER R_PAREN R_PAREN {
          $$ = new Command(*$3, std::make_unique<Expression>(*$7));
        }
       | EXIT L_PAREN Expression R_PAREN {
          $$ = new Command(CommandType::EXIT, std::unique_ptr<Expression>($3));
        }
       | RETURN L_PAREN Expression R_PAREN {
          $$ = new Command(CommandType::RETURN, std::unique_ptr<Expression>($3));
        }
       | RETURN L_PAREN R_PAREN {
          $$ = new Command(CommandType::RETURN);
        }
       | Expression {
          $$ = new Command(CommandType::EXPRESSION, std::unique_ptr<Expression>($1));
        }
       ;

Condition: Expression { $$ = $1; }
         ;

Assign: Expression { $$ = $1; }
      ;

Expressions: Expression COMMA Expressions {
              (*$3).emplace_front(std::move($1));
              $$ = $3;
            }
           | Expression {
              $$ = new std::deque<std::unique_ptr<Expression>>();
              (*$$).emplace_front(std::move($1));
            }
           ;

Expression: TernaryExpression { $$ = $1; }
          | BinaryExpression { $$ = $1; }
          | UnaryExpression { $$ = $1; }
          | Integer { $$ = new Expression($1); }
          | STRING { $$ = new Expression(*$1); }
          | CHARACTER { $$ = new Expression(*$1); }
          | IDENTIFIER { $$ = new Expression(*$1); }
          | IDENTIFIER L_PAREN Expressions R_PAREN { $$ = new Expression(*$1,std::unique_ptr<std::deque<std::unique_ptr<Expression>>>($3)); }
          | IDENTIFIER L_SQUARE_BRACKET Expression R_SQUARE_BRACKET { $$ = new Expression(*$1,std::unique_ptr<Expression>($3)); }
          ;

TernaryExpression: TERNARY_OPERATOR L_PAREN Expression COMMA Expression COMMA Expression R_PAREN {
                    $$ = new Expression(std::unique_ptr<Expression>($3), std::unique_ptr<Expression>($5), std::unique_ptr<Expression>($7));
                  }
                 ;

BinaryExpression: Operator L_PAREN Expression COMMA Expression R_PAREN {
                    $$ = new Expression(std::unique_ptr<Expression>($3), std::unique_ptr<Expression>($5), $1);
                  }
                ;

UnaryExpression:  Operator L_PAREN Expression R_PAREN {
                  $$ = new Expression(std::unique_ptr<Expression>($3), $1);
                }
               |  L_PAREN Expression R_PAREN INC {
                  $$ = new Expression(std::unique_ptr<Expression>($2), OperatorType::INC);
                }
               |  L_PAREN Expression R_PAREN DEC {
                  $$ = new Expression(std::unique_ptr<Expression>($2), OperatorType::DEC);
                }
               ;

Operator: PLUS { $$ = OperatorType::PLUS; }
        | MINUS { $$ = OperatorType::MINUS; }
        | MULTIPLY { $$ = OperatorType::MULTIPLY; }
        | DIVIDE { $$ = OperatorType::DIVIDE; }
        | REMAINDER { $$ = OperatorType::REMAINDER; }
        | BITWISE_AND { $$ = OperatorType::BITWISE_AND; }
        | BITWISE_OR { $$ = OperatorType::BITWISE_OR; }
        | BITWISE_XOR { $$ = OperatorType::BITWISE_XOR; }
        | LOGICAL_AND { $$ = OperatorType::LOGICAL_AND; }
        | LOGICAL_OR { $$ = OperatorType::LOGICAL_OR; }
        | EQUAL { $$ = OperatorType::EQUAL; }
        | NOT_EQUAL { $$ = OperatorType::NOT_EQUAL; }
        | LESS_THAN { $$ = OperatorType::LESS_THAN; }
        | GREATER_THAN { $$ = OperatorType::GREATER_THAN; }
        | LESS_EQUAL { $$ = OperatorType::LESS_EQUAL; }
        | GREATER_EQUAL { $$ = OperatorType::GREATER_EQUAL; }
        | R_SHIFT { $$ = OperatorType::R_SHIFT; }
        | L_SHIFT { $$ = OperatorType::L_SHIFT; }
        | ASSIGN { $$ = OperatorType::ASSIGN; }
        | ADD_ASSIGN { $$ = OperatorType::ADD_ASSIGN; }
        | MINUS_ASSIGN { $$ = OperatorType::MINUS_ASSIGN; }
        | INC { $$ = OperatorType::INC; }
        | DEC { $$ = OperatorType::DEC; }
        | BITWISE_NOT { $$ = OperatorType::BITWISE_NOT; }
        | NOT { $$ = OperatorType::NOT; }
        ;

ReturnType: Type { $$ = $1; }
          | VOID { $$ = nullptr; }
          ;

Type: INT StarLoop {
        $$ = new Type(Type::INT, $2, nullptr);
      }
    | CHAR StarLoop {
        $$ = new Type(Type::CHAR, $2, nullptr);
      }
    | VOID MULTIPLY StarLoop {
        $$ = new Type(Type::VOID_POINTER, $3+1 , nullptr);
      }
    | INT Dimensions {
        $$ = new Type(Type::ARRAY_INT, 1, std::unique_ptr<std::deque<int>>($2));
      }
    | CHAR Dimensions {
        $$ = new Type(Type::ARRAY_CHAR, 1, std::unique_ptr<std::deque<int>>($2));
      }
    ;

Dimensions: Dimension Dimensions  {
              (*$2).insert((*$2).begin(),(*$1).begin(), (*$1).end());
              delete $1;
              $$ = $2;
            }
          | Dimension { $$ = $1; }
          ;

Dimension: L_SQUARE_BRACKET INTEGER R_SQUARE_BRACKET { $$ = new std::deque<int> { $2 }; }

StarLoop: MULTIPLY StarLoop { $$ = ++$2; }
        | { $$ = 0; }
        ;

Integer: INTEGER { $$ = $1; }
      | MINUS INTEGER { $$ = -$2; }
      | PLUS INTEGER { $$ = +$2; }
      ;
%%

void yyerror(const char *) {
}