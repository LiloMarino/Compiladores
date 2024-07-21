%{
#include "lexer.hpp"
%}

%option noyywrap

%%

[a-z][a-z0-9]*   { return IDENTIFIER; }
[0-9]+           { return NUMBER; }
.                { return yytext[0]; }

%%
