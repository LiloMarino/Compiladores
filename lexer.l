%option noyywrap
%{
#include "tokens.hpp"
%}

%%

[a-z][a-z0-9]* {return static_cast<int>(Token::VALIDO);}

[+-] {return static_cast<int>(Token::VALIDO);}

[0-9]+ {return static_cast<int>(Token::VALIDO);}

[0-9]+e[+-]?[0-9]+ {return static_cast<int>(Token::VALIDO);}

\n { /* Ignora quebra de linha */ }

<<EOF>> {return static_cast<int>(Token::END_OF_FILE);}

. {return static_cast<int>(Token::INVALIDO);}

%%
