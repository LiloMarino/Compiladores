%option noyywrap
%option yylineno
%x COMMENT
%{
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

// Variáveis globais
bool firstToken = true;
int coluna = 1;
int inicioLinha;
int inicioColuna;
char result[256];

// Print Token
void print(char *token)
{
    if (firstToken)
    {
        printf("%s", token);
        firstToken = false;
    }
    else
    {
        printf("\n%s", token);
    }
    coluna += strlen(yytext);
}

// Controle de exceções
enum ExceptionLevel
{
    WARNING,
    ERROR,
};

void throwException(enum ExceptionLevel level, bool lexical, int line, int column, char *message)
{
    char text[256] = "";
    if (level == WARNING)
    {
        strcat(text, "warning:");
    }
    else if (level == ERROR)
    {
        strcat(text, "error:");
    }
    if (lexical)
    {
        strcat(text, "lexical:");
    }

    if (!firstToken)
    {
        printf("\n");
    }
    printf("%s%d:%d: %s", text, line, column, message);
    firstToken = false;
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
        sprintf(result, "'%s' within block comment", yytext);
        throwException(WARNING,false,yylineno,coluna,result);
        coluna += strlen(yytext);
        if (next == '\n')
        {
            yylineno++;
        }
        unput(next);
    }
}
<COMMENT>. { coluna += strlen(yytext); }
<COMMENT>\n { coluna = 1; }
<COMMENT><<EOF>> { 
    throwException(ERROR,true,inicioLinha,inicioColuna,"unterminated comment");
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
[a-zA-Z_][0-9a-zA-Z_]* {if (yyleng <= 255)
                        {
                            sprintf(result,"IDENTIFIER(%s)",yytext);
                            print(result);
                        }
                        else
                        {
                            throwException(ERROR, true, yylineno, coluna," identifier too long");
                        }
                       }
.                      {throwException(ERROR,true,yylineno,coluna,yytext);}
%%

int main() {
    yylex();
    return 0;
}