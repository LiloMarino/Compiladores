%{
#include <iostream>
#include "utils.hpp"

extern int yylex();
extern int yyparse();
extern void yyerror(const char *s);
extern int yylineno;

SymbolTable symbolTable;
%}

%union {
    char* str;
}

%token <str> ID
%token INT FLOAT CHAR
%type <str> T

%%

S : D D_prime '$' { 
      std::cout << "All Identifiers on Hash." << std::endl; 
      symbolTable.clearTable(); 
    }
  ;

D_prime : D D_prime
        | 
        ;

D : T I
  ;

T : INT { $$ = "int"; }
  | FLOAT { $$ = "float"; }
  | CHAR { $$ = "char"; }
  ;

I : ID {
          std::string errorMsg;
          if (!symbolTable.addSymbol($1, $<str>1, errorMsg)) {
              std::cout << yylineno << ": " << errorMsg << std::endl;
          }
          free($1);  // Liberar memória alocada
      }
    | I ',' ID {
          std::string errorMsg;
          if (!symbolTable.addSymbol($3, $<str>-2, errorMsg)) {
              std::cout << yylineno << ": " << errorMsg << std::endl;
          }
          free($3);  // Liberar memória alocada
      }
    ;

%%

void yyerror(const char *s) {
    std::cerr << "Parse error: " << s << std::endl;
}