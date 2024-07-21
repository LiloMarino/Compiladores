#ifndef LEXER_HPP
#define LEXER_HPP

extern int yylex();
extern char* yytext;

enum Token {
    IDENTIFIER = 256,
    NUMBER
};

#endif
