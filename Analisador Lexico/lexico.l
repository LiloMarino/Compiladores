%option noyywrap
%option yylineno

%{
    #include <stdio.h>    
%}
%%
"void"      {printf("VOID");}
"int"       {printf("INT");}
"char"      {printf("CHAR");}
"return"    {printf("RETURN");}
"break"     {printf("BREAK");}
"switch"    {printf("SWITCH");}
"case"      {printf("CASE");}
"default"   {printf("DEFAULT");}
"do"        {printf("DO");}
"while"     {printf("WHILE");}
"for"       {printf("FOR");}
"if"        {printf("IF");}
"else"      {printf("ELSE");}
"typedef"   {printf("TYPEDEF");}
"struct"    {printf("STRUCT");}
"+"         {printf("PLUS");}
"-"         {printf("MINUS");}
"*"         {printf("MULTIPLY");}
"/"         {printf("DIV");}
"%"         {printf("REMAINDER");}
"++"        {printf("INC");}
"--"        {printf("DEC");}
"&"         {printf("BITWISE_AND");}
"|"         {printf("BITWISE_OR");}
"~"         {printf("BITWISE_NOT");}
"^"         {printf("BITWISE_XOR");}
"!"         {printf("NOT");}
"&&"        {printf("LOGICAL_AND");}
"||"        {printf("LOGICAL_OR");}
"=="        {printf("EQUAL");}
"!="        {printf("NOT_EQUAL");}
"<"         {printf("LESS_THAN");}
">"         {printf("GREATER_THAN");}
"<="        {printf("LESS_EQUAL");}
">="        {printf("GREATER_EQUAL");}
">>"        {printf("R_SHIFT");}
"<<"        {printf("L_SHIFT");}
"="         {printf("ASSIGN");}
"+="        {printf("ADD_ASSIGN");}
"-="        {printf("MINUS_ASSIGN");}
";"         {printf("SEMICOLON");}
","         {printf("COMMA");}
":"         {printf("COLON");}
"("         {printf("L_PAREN");}
")"         {printf("R_PAREN");}
"{"         {printf("L_CURLY_BRACKET");}
"}"         {printf("R_CURLY_BRACKET");}
"["         {printf("L_SQUARE_BRACKET");}
"]"         {printf("R_SQUARE_BRACKET");}
"?"         {printf("TERNARY_CONDITIONAL");}
"#"         {printf("NUMBER_SIGN");}
"->"        {printf("POINTER");}
"printf"    {printf("PRINTF");}
"scanf"     {printf("SCANF");}
"define"    {printf("DEFINE");}
"exit"      {printf("EXIT");}

%%

int main() {
    yylex();
    return 0;
}