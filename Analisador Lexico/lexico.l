%option noyywrap
%option yylineno
%x COMMENT
%{
    #include <stdio.h>
    #include <stdbool.h>
    #include <string.h>
    bool haveTextBefore = false;
    bool firstToken = true;
    int coluna = 1;
    int inicioLinha;
    int inicioColuna;
    char result[256];
    void print(char * token)
    {
        if(firstToken)
        {
            printf("%s",token);
            firstToken = false;
        }
        else
        {
            printf("\n%s",token);
        }
        haveTextBefore = true;
        coluna += strlen(yytext);
    }
    void throwError(char *message)
    {
        if(firstToken)
        {
            printf("error:%s:%d:%d: %s",message,yylineno,coluna,yytext);
            firstToken = false;
        }
        else
        {
            printf("\nerror:%s:%d:%d: %s",message,yylineno,coluna,yytext);
        }
        haveTextBefore = true;
        coluna += strlen(yytext);
    }
    void throwWarning(char *message)
    {
        if(firstToken)
        {
            printf("warning:%d:%d: %s",message,yylineno,coluna,yytext);
            firstToken = false;
        }
        else
        {
            printf("\nwarning:%d:%d: %s",message,yylineno,coluna,yytext);
        }
        haveTextBefore = true;
        coluna += strlen(yytext);
    }
%}
%%

"/*" { 
    BEGIN(COMMENT); 
    inicioLinha = yylineno;
    inicioColuna = coluna; 
    coluna += strlen(yytext); 
}
<COMMENT>"*/" {
    BEGIN(INITIAL); 
    coluna += strlen(yytext); 
} 
<COMMENT>"/*" {
    char next = input();
    if (next == '\n')
    {
        yylineno--;
    }
    if (next == '/')
    {
        BEGIN(INITIAL);
        coluna += strlen(yytext) + 1;
    }
    else
    {
        if (haveTextBefore)
        {
            printf("\n");
        }
        throwWarning(sprintf(result, "'%s' within block comment", yytext));
        coluna += strlen(yytext);
        if (next == '\n')
        {
            yylineno++;
        }
        unput(next);
    }
    haveTextBefore = 1;
}
<COMMENT>. { coluna += strlen(yytext); }
<COMMENT>\n { coluna = 1; }
<COMMENT><<EOF>> { 
    if (haveTextBefore) 
    {
        printf("\n");
    }
    printf("error:lexical:%d:%d: unterminated comment", inicioLinha, inicioColuna); 
    exit(1);
}
"//".*\n               {printf("comentario de linha\n"); /*Ignora comentário */ }
\n                     {/*Ignora quebra de linha*/}
[ ]                    {coluna++;}
"void"                 {print("VOID");}
"int"                  {print("INT");}
"char"                 {print("CHAR");}
"return"               {print("RETURN");}
"break"                {print("BREAK");}
"switch"               {print("SWITCH");}
"case"                 {print("CASE");}
"default"              {print("DEFAULT");}
"do"                   {print("DO");}
"while"                {print("WHILE");}
"for"                  {print("FOR");}
"if"                   {print("IF");}
"else"                 {print("ELSE");}
"typedef"              {print("TYPEDEF");}
"struct"               {print("STRUCT");}
"+"                    {print("PLUS");}
"-"                    {print("MINUS");}
"*"                    {print("MULTIPLY");}
"/"                    {print("DIV");}
"%"                    {print("REMAINDER");}
"++"                   {print("INC");}
"--"                   {print("DEC");}
"&"                    {print("BITWISE_AND");}
"|"                    {print("BITWISE_OR");}
"~"                    {print("BITWISE_NOT");}
"^"                    {print("BITWISE_XOR");}
"!"                    {print("NOT");}
"&&"                   {print("LOGICAL_AND");}
"||"                   {print("LOGICAL_OR");}
"=="                   {print("EQUAL");}
"!="                   {print("NOT_EQUAL");}
"<"                    {print("LESS_THAN");}
">"                    {print("GREATER_THAN");}
"<="                   {print("LESS_EQUAL");}
">="                   {print("GREATER_EQUAL");}
">>"                   {print("R_SHIFT");}
"<<"                   {print("L_SHIFT");}
"="                    {print("ASSIGN");}
"+="                   {print("ADD_ASSIGN");}
"-="                   {print("MINUS_ASSIGN");}
";"                    {print("SEMICOLON");}
","                    {print("COMMA");}
":"                    {print("COLON");}
"("                    {print("L_PAREN");}
")"                    {print("R_PAREN");}
"{"                    {print("L_CURLY_BRACKET");}
"}"                    {print("R_CURLY_BRACKET");}
"["                    {print("L_SQUARE_BRACKET");}
"]"                    {print("R_SQUARE_BRACKET");}
"?"                    {print("TERNARY_CONDITIONAL");}
"#"                    {print("NUMBER_SIGN");}
"->"                   {print("POINTER");}
"printf"               {print("PRINTF");}
"scanf"                {print("SCANF");}
"define"               {print("DEFINE");}
"exit"                 {print("EXIT");}
[a-zA-Z_][0-9a-zA-Z_]* {if(yyleng <= 255){print("IDENTIFIER");}}
.                      {throwError("lexical");}
%%

int main() {
    yylex();
    return 0;
}