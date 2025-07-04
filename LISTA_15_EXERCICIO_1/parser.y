%code requires {
// BISON BUGADO PRECISA FAZER ISSO AQUI
#include <iostream>
#include <string>
#include <memory>
#include "utils.hpp"
}

%{
extern int yylex();
void yyerror(const char *s);
bool error = false;
%}

%union {
    std::string *str;
}

%token INT FLOAT CHAR
%token <str> ID
%type <str> T
%type <str> I

%%

S : D D_prime '$' { 
      if(!error)
      {
        showMessage("All Identifiers on Hash.");
      }
      symbolTable.clearTable(); 
      error = false;
      return 0;
    }
  ;

D_prime : D D_prime
        | 
        ;

D : T I {
          symbolTable.addSymbol(*$1, *$2);
          delete $1;
          delete $2;
      }
  ;

T : INT { $$ = new std::string("int"); }
  | FLOAT { $$ = new std::string("float"); }
  | CHAR { $$ = new std::string("char"); }
  ;

I : ID        { $$ = $1; }
  | I ',' ID  { $$ = new std::string(*$1 + "," + *$3); delete $1; delete $3; }
  ;

%%

void yyerror(const char *s) { /* Ignora os erros */ }
