%code requires {
// BISON BUGADO PRECISA FAZER ISSO AQUI
#include "utils.hpp"
}

%{
extern int yylex();
%}

%token-table
%token 
PLUS
MINUS
MULTIPLY
DIVIDE
EXPONENT
MODULO
LEFT_PAREN
RIGHT_PAREN
SIN
COS
TAN
ABS
INTEGER
REAL_NUMBER
VARIABLE
IDENTIFIER
PI_CONSTANT
EULER_CONSTANT

%start Program

%%

Program : { return 0; };

%%

const char* getTokenName(int token) {
    // Converte o valor do token para yysymbol_kind_t e chama yysymbol_name
    return yysymbol_name(YYTRANSLATE(token));
}