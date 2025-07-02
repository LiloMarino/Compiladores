%{
#include <iostream>
#include "utils.hpp"
void yyerror(const char *s);
extern int yylex();
extern int column;
extern char* yytext;
extern bool lexical_error;
%}

%token NUMBER REAL_NUMBER VARIABLE PLUS MINUS MULTIPLY DIVIDE POW REST L_PAREN R_PAREN SEN COS TAN ABS
%start input

%left PLUS MINUS
%left MULTIPLY DIVIDE REST
%left POW
%right UMINUS

%%

input:
      expr '\n' { if (!lexical_error) std::cout << "EXPRESSAO CORRETA" << std::endl; }
    | error '\n' { if (!lexical_error) yyerror("Erro de sintaxe"); }
    ;

expr: expr PLUS expr 
    | expr MINUS expr 
    | expr MULTIPLY expr 
    | expr DIVIDE expr
    | expr REST expr
    | expr POW expr
    | MINUS expr %prec UMINUS
    | PLUS expr %prec UMINUS
    | L_PAREN expr R_PAREN 
    | SEN L_PAREN expr R_PAREN 
    | COS L_PAREN expr R_PAREN 
    | TAN L_PAREN expr R_PAREN 
    | ABS L_PAREN expr R_PAREN 
    | NUMBER 
    | REAL_NUMBER 
    | VARIABLE
    ;

%%

void yyerror(const char *s) {
    std::cout << "Erro de sintaxe na coluna [" << column << "]: " << yytext << std::endl;
}
