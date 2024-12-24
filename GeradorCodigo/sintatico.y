%code requires {
// Necessário para evitar problemas de inclusão
// Coloca na .h
#include <iostream>
}

%{
// Coloca na .c
extern int yylex();
void yyerror(const char *msg);
%}

%union {
    double value;
    std::string* str;
}

%token <str> IDENTIFIER STRING CHARACTER
%token <value> INTEGER
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

AstLoop: Definition AstLoop
       |
       ;

Definition: Constant
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
       | WHILE L_PAREN Condition COMMA Commands R_PAREN
       | FOR L_PAREN Assign COMMA Condition COMMA Assign COMMA Commands R_PAREN
       | PRINTF L_PAREN STRING COMMA Expressions R_PAREN
       | SCANF L_PAREN STRING COMMA BITWISE_AND L_PAREN IDENTIFIER R_PAREN R_PAREN
       | EXIT L_PAREN Expression R_PAREN
       | RETURN L_PAREN Expression R_PAREN
       | Expression
       ;

Condition: Expression
         ;

Assign:
      ;

Expressions: Expression COMMA Expressions
           | Expression
           ;

Expression: BinaryOperator L_PAREN Expression COMMA Expression R_PAREN
          | UnaryOperator L_PAREN Expression R_PAREN
          | L_PAREN Expression R_PAREN UnaryOperator
          | TERNARY_OPERATOR L_PAREN Expression COMMA Expression COMMA Expression R_PAREN
          ;

BinaryOperator: PLUS
              | MINUS
              | MULTIPLY
              | DIVIDE
              | REMAINDER
              | BITWISE_AND
              | BITWISE_OR
              | BITWISE_XOR
              | LOGICAL_AND
              | LOGICAL_OR
              | EQUAL
              | NOT_EQUAL
              | LESS_THAN
              | GREATER_THAN
              | LESS_EQUAL
              | GREATER_EQUAL
              | R_SHIFT
              | L_SHIFT
              | ASSIGN
              | ADD_ASSIGN
              | MINUS_ASSIGN
              ;

UnaryOperator: PLUS
             | MINUS
             | MULTIPLY
             | INC
             | DEC
             | BITWISE_NOT
             | NOT
             ;

ReturnType: Type
          | VOID
          ;

Type: INT StarLoop
    | CHAR StarLoop
    | VOID MULTIPLY StarLoop
    ;

StarLoop: MULTIPLY StarLoop
        | 
        ;

Integer: INTEGER { $$ = $1; }
      | MINUS INTEGER { $$ = -$2; }
      | PLUS INTEGER { $$ = +$2; }
      ;
%%

void yyerror(const char *msg) {
    std::cout << msg << std::endl;
}