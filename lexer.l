%option noyywrap
%{
#include "tokens.hpp"
%}

%%

"/*"(([^*]|\n)+|.)"*/"      { return static_cast<int>(Token::COMMENTLINE);}
"//".*\n                    { return static_cast<int>(Token::COMMENTLINE);}
"#include"|"#define"        { return static_cast<int>(Token::PREPROCDIR); }
";"                         { return static_cast<int>(Token::SEMICOLON); }
^[ \t]+$                      { return static_cast<int>(Token::BLANKLINE); }
.+\n                        { return static_cast<int>(Token::NB_NC_LINE); }
<<EOF>>                     { return static_cast<int>(Token::END_OF_FILE); }
.                           { return static_cast<int>(Token::ERROR); }

%%
