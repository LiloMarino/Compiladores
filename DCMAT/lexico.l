%option noyywrap
%option yylineno
%{
#include <iostream>
#include <string>
#include <cstring>
#include "utils.hpp"
#include "sintatico.tab.h"

// Variáveis globais
int yycolno = 1;

%}
%%
.                      { throwException(ExceptionLevel::ERROR, ExceptionType::LEXICAL, yylineno, yycolno, yytext); }
%%