%option noyywrap
%option yylineno
%{
#include "tokens.hpp"
#include "stats.hpp"
bool hasBlank = true;
bool isBlankLine = false;
int startMultiLineComment = 0;
%}
%x COMMENT

%%

";" { atual.semi_colons++; }
"/*" { BEGIN(COMMENT); startMultiLineComment = yylineno; }
<COMMENT>"\n" { 
    if (isBlankLine) {
        atual.blank_lines++;
        hasBlank = false;
    }
    isBlankLine = true;
}
<COMMENT>";" { atual.semi_colons++; }
<COMMENT>. { isBlankLine = false; }
<COMMENT>"*/" { 
    BEGIN(INITIAL); 
    atual.lines_with_comments += yylineno - startMultiLineComment; 
    atual.lines_with_comments++; 
    int c = yyinput();
    if (c == '\n') {
        atual.blank_lines++;
    } else {
        yyunput(c, yytext);
    }
}
"//".*\n { atual.lines_with_comments++; }
"\n" {
    if (isBlankLine) {
        atual.blank_lines++;
        hasBlank = false;
    }
    isBlankLine = true;
}
. { isBlankLine = false; }
"#"("include"|"define") { atual.prepoc_direct++; }
<<EOF>> { 
    atual.total_lines = yylineno;
    atual.nb_nc_lines = yylineno - atual.blank_lines - atual.lines_with_comments; 
    return static_cast<int>(Token::END_OF_FILE); 
}

%%
