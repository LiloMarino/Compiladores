%option noyywrap
%{
#include "tokens.hpp"
%}

%%

"aba"+          { return static_cast<int>(Token::ACTION1); }
"a""b"*"a"      { return static_cast<int>(Token::ACTION2); }
"a"|"b"         { return static_cast<int>(Token::ACTION3); }
\n              { /* Ignora quebra de linha */ }
<<EOF>>         { return static_cast<int>(Token::END_OF_FILE); }
.               { return static_cast<int>(Token::ERROR); }

%%
