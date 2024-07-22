%option noyywrap
%{
#include "tokens.hpp"
%}

%%
"token1" { 
    return static_cast<int>(Token::STRING1); 
}
"token2" { 
    return static_cast<int>(Token::STRING2); 
}
. {return 0;}
%%

// Outras definições e funções, como o método main para rodar o lexer, se necessário
