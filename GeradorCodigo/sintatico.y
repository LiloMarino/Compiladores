%code requires {
// Necessário para evitar problemas de inclusão
// Coloca na .h
#include <iostream>
#include "expression.hpp"
}

%{
// Coloca na .c
extern int yylex();
void yyerror(const char *msg);
%}
%debug
%verbose
%union {
    int value;
    std::string* str;
    OperatorType op;
}

%token <str> IDENTIFIER STRING CHARACTER
%token <value> INTEGER
%type <op> Operator
%type <value> Integer

%token GLOBAL VARIABLE CONSTANT PARAMETER VALUE RETURN_TYPE TYPE VOID INT CHAR FUNCTION END_FUNCTION RETURN DO_WHILE 
WHILE FOR IF PRINTF SCANF EXIT PLUS MINUS MULTIPLY DIVIDE REMAINDER INC DEC BITWISE_AND BITWISE_OR BITWISE_NOT 
BITWISE_XOR NOT LOGICAL_AND LOGICAL_OR EQUAL NOT_EQUAL LESS_THAN GREATER_THAN LESS_EQUAL GREATER_EQUAL R_SHIFT L_SHIFT 
ASSIGN ADD_ASSIGN MINUS_ASSIGN SEMICOLON COMMA COLON L_PAREN R_PAREN L_SQUARE_BRACKET R_SQUARE_BRACKET 
TERNARY_OPERATOR EOF_TOKEN

%start AstStart

%%

AstStart: AstLoop EOF_TOKEN
        ;

AstLoop: Declaration AstLoop
       |
       ;

Declaration: Constant
           | GlobalVariable
           | Function
           ;

Constant: CONSTANT COLON IDENTIFIER VALUE COLON Integer
        ;

GlobalVariable: GLOBAL VARIABLE COLON IDENTIFIER TYPE COLON Type
              ;

Function: FUNCTION COLON IDENTIFIER RETURN_TYPE COLON ReturnType Parameters Variables Commands END_FUNCTION
        ;

Parameters: Parameter Parameters
          |
          ;

Parameter: PARAMETER COLON IDENTIFIER TYPE COLON Type

Variables: LocalVariable Variables
         |
         ;

LocalVariable: VARIABLE COLON IDENTIFIER TYPE COLON Type
             ;

Commands: Command SEMICOLON Commands
        | Command
        ;

Command: DO_WHILE L_PAREN Commands COMMA Condition R_PAREN
       | IF L_PAREN Condition COMMA Commands COMMA Commands R_PAREN
       | IF L_PAREN Condition COMMA Commands R_PAREN
       | WHILE L_PAREN Condition COMMA Commands R_PAREN
       | FOR L_PAREN Assign COMMA Condition COMMA Assign COMMA Commands R_PAREN
       | PRINTF L_PAREN STRING COMMA Expressions R_PAREN
       | PRINTF L_PAREN STRING R_PAREN
       | SCANF L_PAREN STRING COMMA BITWISE_AND L_PAREN IDENTIFIER R_PAREN R_PAREN
       | EXIT L_PAREN Expression R_PAREN
       | RETURN L_PAREN Expression R_PAREN
       | RETURN L_PAREN R_PAREN
       | Expression
       ;

Condition: Expression
         ;

Assign: Expression
      ;

Expressions: Expression COMMA Expressions
           | Expression
           ;

Expression: TernaryExpression 
          | BinaryExpression
          | UnaryExpression
          | INTEGER
          | STRING
          | CHARACTER
          | IDENTIFIER
          | IDENTIFIER L_PAREN Expressions R_PAREN
          | IDENTIFIER L_SQUARE_BRACKET Expression R_SQUARE_BRACKET
          ;

TernaryExpression: TERNARY_OPERATOR L_PAREN Expression COMMA Expression COMMA Expression R_PAREN

BinaryExpression: Operator L_PAREN Expression COMMA Expression R_PAREN
                ;

UnaryExpression:  Operator L_PAREN Expression R_PAREN
               |  L_PAREN Expression R_PAREN INC
               |  L_PAREN Expression R_PAREN DEC
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

ReturnType: Type
          | VOID
          ;

Type: INT StarLoop
    | CHAR StarLoop
    | VOID MULTIPLY StarLoop
    | INT Dimensions
    | CHAR Dimensions
    ;

Dimensions: Dimension Dimensions
          | Dimension
          ;

Dimension: L_SQUARE_BRACKET INTEGER R_SQUARE_BRACKET

StarLoop: MULTIPLY StarLoop
        | 
        ;

Integer: INTEGER { $$ = $1; }
      | MINUS INTEGER { $$ = -$2; }
      | PLUS INTEGER { $$ = +$2; }
      ;
%%

void yyerror(const char *) {
}