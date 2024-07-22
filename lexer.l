%option noyywrap
%{
#include "tokens.hpp"
%}

%%

"if"            { return static_cast<int>(Token::IF); }
[a-z][a-z0-9]*  { return static_cast<int>(Token::ID); }
[0-9]+          { return static_cast<int>(Token::NUM); }
[0-9]+"."[0-9]* { return static_cast<int>(Token::REAL); }
"."[0-9]+       { return static_cast<int>(Token::REAL); }
--[a-z]*$       { return static_cast<int>(Token::COMMENT); }
[ \t]+          { return static_cast<int>(Token::WHITE_SPACE); }
\n              { /* Ignora quebra de linha não associada a comment */ }
<<EOF>>         { return static_cast<int>(Token::END_OF_FILE); }
.               { return static_cast<int>(Token::ERROR); }

%%
