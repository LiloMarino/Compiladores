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


%%

S : '$' { 
    }
  ;

%%

void yyerror(const char *s) { /* Ignora os erros */ }
