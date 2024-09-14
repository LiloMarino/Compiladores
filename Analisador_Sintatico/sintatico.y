%code requires {
// BISON BUGADO PRECISA FAZER ISSO AQUI
#include "utils.hpp"
}

%{
extern int yylex();
%}

%token VOID INT CHAR RETURN BREAK SWITCH CASE DEFAULT DO WHILE FOR IF ELSE TYPEDEF STRUCT PLUS MINUS MULTIPLY 
%token DIVIDE MODULO INCREMENT DECREMENT BITWISE_AND BITWISE_OR BITWISE_NOT BITWISE_XOR NOT LOGICAL_AND 
%token LOGICAL_OR EQUAL NOT_EQUAL LESS_THAN GREATER_THAN LESS_EQUAL GREATER_EQUAL RIGHT_SHIFT LEFT_SHIFT 
%token ASSIGN PLUS_ASSIGN MINUS_ASSIGN SEMICOLON COMMA COLON LEFT_PAREN RIGHT_PAREN LEFT_CURLY_BRACKET 
%token RIGHT_CURLY_BRACKET LEFT_SQUARE_BRACKET RIGHT_SQUARE_BRACKET QUESTION_MARK HASH ARROW PRINTF SCANF 
%token DEFINE EXIT IDENTIFIER NUM_OCTAL NUM_HEXA NUM_INTEGER STRING_LITERAL CHARACTER_LITERAL

%start Program

%%

Program: Declaration ProgramLoop
       | Function ProgramLoop
       ;

ProgramLoop: Declaration ProgramLoop
           | Function ProgramLoop
           | 
           ;

Declaration: HASH DEFINE IDENTIFIER Expression
           |  VariableDeclaration
           |  PrototypeDeclaration
           ;

Function: Type StarLoop IDENTIFIER Parameters LEFT_CURLY_BRACKET Commands RIGHT_CURLY_BRACKET
        | Type StarLoop IDENTIFIER Parameters LEFT_CURLY_BRACKET VariableDeclaration DeclarationLoop Commands RIGHT_CURLY_BRACKET
        ;

DeclarationLoop: VariableDeclaration DeclarationLoop
               |
               ;

StarLoop: MULTIPLY StarLoop
        | 
        ;

VariableDeclaration: Type StarLoop IDENTIFIER IndexLoop ASSIGN AttributionExpression VariableDeclarationLoop SEMICOLON
                   | Type StarLoop IDENTIFIER IndexLoop VariableDeclarationLoop SEMICOLON
                   ;

VariableDeclarationLoop: COMMA StarLoop IDENTIFIER IndexLoop ASSIGN AttributionExpression VariableDeclarationLoop
                       | COMMA StarLoop IDENTIFIER IndexLoop VariableDeclarationLoop
                       |
                       ;

IndexLoop: LEFT_SQUARE_BRACKET Expression RIGHT_SQUARE_BRACKET IndexLoop
         |
         ;

PrototypeDeclaration: Type StarLoop IDENTIFIER Parameters SEMICOLON
                    ;

Parameters: LEFT_PAREN RIGHT_PAREN
          | LEFT_PAREN Type StarLoop IDENTIFIER IndexLoop ParametersLoop RIGHT_PAREN

ParametersLoop: COMMA Type StarLoop IDENTIFIER IndexLoop ParametersLoop
              |
              ;
Type: INT
    | CHAR
    | VOID
    ;

Block: LEFT_CURLY_BRACKET Commands RIGHT_CURLY_BRACKET 
     ;

Commands: CommandList CommandsLoop
        ;

CommandsLoop: CommandList CommandsLoop
            | 
            ;

CommandList: DO Block WHILE LEFT_PAREN Expression RIGHT_PAREN SEMICOLON
           | IF LEFT_PAREN Expression RIGHT_PAREN Block ELSE Block
           | IF LEFT_PAREN Expression RIGHT_PAREN Block
           | WHILE LEFT_PAREN Expression RIGHT_PAREN Block
           | FOR LEFT_PAREN OptionalExpression SEMICOLON OptionalExpression SEMICOLON OptionalExpression RIGHT_PAREN Block
           | PRINTF LEFT_PAREN STRING_LITERAL ExpressionLoop RIGHT_PAREN SEMICOLON
           | SCANF LEFT_PAREN STRING_LITERAL COMMA BITWISE_AND IDENTIFIER RIGHT_PAREN SEMICOLON
           | EXIT LEFT_PAREN Expression RIGHT_PAREN SEMICOLON
           | RETURN Expression SEMICOLON
           | RETURN SEMICOLON
           | Expression SEMICOLON
           | SEMICOLON
           | Block
           ;

OptionalExpression: Expression
                  | 
                  ;

ExpressionLoop: COMMA Expression ExpressionLoop
              |
              ;

Expression: AttributionExpression
          | Expression COMMA AttributionExpression
          ;

AttributionExpression: ConditionalExpression
                     | UnaryExpression PLUS_ASSIGN AttributionExpression
                     | UnaryExpression MINUS_ASSIGN AttributionExpression
                     | UnaryExpression ASSIGN AttributionExpression
                     ;

ConditionalExpression: LogicalOR
                     | LogicalOR QUESTION_MARK Expression COLON ConditionalExpression
                     ;

LogicalOR: LogicalAND
         | LogicalOR LOGICAL_OR LogicalAND
         ;
         
LogicalAND: ORExpression
          | LogicalAND LOGICAL_AND ORExpression
          ;

ORExpression: XORExpression
            | ORExpression BITWISE_OR XORExpression
            ;

XORExpression: ANDExpression
             | XORExpression BITWISE_XOR ANDExpression
             ;

ANDExpression: EqualityExpression
             | ANDExpression BITWISE_AND EqualityExpression
             ;

EqualityExpression: RelationalExpression
                  | EqualityExpression EQUAL RelationalExpression
                  | EqualityExpression NOT_EQUAL RelationalExpression
                  ;

RelationalExpression: ShiftExpression
                    | RelationalExpression GREATER_THAN ShiftExpression
                    | RelationalExpression GREATER_EQUAL ShiftExpression
                    | RelationalExpression LESS_THAN ShiftExpression
                    | RelationalExpression LESS_EQUAL ShiftExpression
                    ;

ShiftExpression: AddictiveExpression
               | ShiftExpression LEFT_SHIFT AddictiveExpression
               | ShiftExpression RIGHT_SHIFT AddictiveExpression
               ;

AddictiveExpression: MultiplicativeExpression
                   | AddictiveExpression PLUS MultiplicativeExpression
                   | AddictiveExpression MINUS MultiplicativeExpression
                   ;

MultiplicativeExpression: CastExpression
                        | MultiplicativeExpression MULTIPLY CastExpression
                        | MultiplicativeExpression DIVIDE CastExpression
                        | MultiplicativeExpression MODULO CastExpression
                        ;

CastExpression: UnaryExpression
              | LEFT_PAREN Type StarLoop RIGHT_PAREN CastExpression
              ;

UnaryExpression: PostFixExpression
               | INCREMENT UnaryExpression
               | DECREMENT UnaryExpression
               | BITWISE_AND CastExpression
               | PLUS CastExpression
               | MINUS CastExpression
               | BITWISE_NOT CastExpression
               | NOT CastExpression
               ;

PostFixExpression: PrimaryExpression
                 | PostFixExpression LEFT_SQUARE_BRACKET Expression RIGHT_SQUARE_BRACKET
                 | PostFixExpression INCREMENT
                 | PostFixExpression DECREMENT
                 | PostFixExpression LEFT_PAREN AttributionExpression AttributionExpressionLoop RIGHT_PAREN
                 | PostFixExpression LEFT_PAREN RIGHT_PAREN
                 ;

AttributionExpressionLoop: COMMA AttributionExpression AttributionExpressionLoop
                         | 
                         ;

PrimaryExpression: IDENTIFIER
                 | Number
                 | CHARACTER_LITERAL
                 | STRING_LITERAL
                 | LEFT_PAREN Expression RIGHT_PAREN
                 ;

Number: NUM_INTEGER
      | NUM_HEXA
      | NUM_OCTAL
      ;

%%
