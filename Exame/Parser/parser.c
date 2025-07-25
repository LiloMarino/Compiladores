#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_TOKENS 1024

// Enum para os tokens
enum TokenType
{
    TOK_ID = 0,   // "id"
    TOK_PLUS,     // "+"
    TOK_STAR,     // "*"
    TOK_PLUSPLUS, // "++"
    TOK_LPAREN,   // "("
    TOK_RPAREN,   // ")"
    TOK_DOLLAR,   // "$"
    TOK_UNKNOWN
};

// Lista de tokens da linha atual
int tokens[MAX_TOKENS];
int token_count = 0;
int token;
int pos = 0;
bool erro = false;

// Função para mapear código para string
const char *token_to_string(int t)
{
    switch (t)
    {
    case TOK_ID:
        return "id";
    case TOK_PLUS:
        return "+";
    case TOK_STAR:
        return "*";
    case TOK_PLUSPLUS:
        return "++";
    case TOK_LPAREN:
        return "(";
    case TOK_RPAREN:
        return ")";
    case TOK_DOLLAR:
        return "$";
    default:
        return "UNKNOWN";
    }
}

// Leitura de linha e parsing de tokens
bool read_tokens_line()
{
    char line[1024];
    if (!fgets(line, sizeof(line), stdin))
        return false;

    token_count = 0;
    for (int i = 0; line[i] && token_count < MAX_TOKENS; i++)
    {
        if (line[i] >= '0' && line[i] <= '9')
        {
            tokens[token_count++] = line[i] - '0';
        }
    }

    return token_count > 0;
}

// Função de erro sintático esperada
void syntax_error(int got, const int *expected, int expected_size)
{
    erro = true;
    printf("ERRO SINTATICO EM: %s ESPERADO: ", token_to_string(got));
    for (int i = 0; i < expected_size; i++)
    {
        if (i > 0)
            printf(", ");
        printf("%s", token_to_string(expected[i]));
    }
}

// Avança para o próximo token
void advance()
{
    if (pos < token_count)
        token = tokens[pos++];
    else
        token = TOK_UNKNOWN;
}

// Verifica se o token atual é o esperado
void eat(int expected)
{
    if (token == expected)
    {
        advance();
    }
    else
    {
        int expected_list[] = {expected};
        syntax_error(token, expected_list, 1);
    }
}

// === Produções ===
void S(), E(), E_(), T(), T_(), F(), P();

void S()
{
    if (token == TOK_ID || token == TOK_LPAREN)
    {
        E();
        if (!erro)
            eat(TOK_DOLLAR);
    }
    else
    {
        int expected[] = {TOK_ID, TOK_LPAREN};
        syntax_error(token, expected, 2);
    }
}

void E()
{
    if (token == TOK_ID || token == TOK_LPAREN)
    {
        T();
        if (!erro)
            E_();
    }
    else
    {
        int expected[] = {TOK_ID, TOK_LPAREN};
        syntax_error(token, expected, 2);
    }
}

void E_()
{
    if (token == TOK_PLUS)
    {
        eat(TOK_PLUS);
        if (!erro)
            T();
        if (!erro)
            E_();
    }
    else if (token == TOK_RPAREN || token == TOK_DOLLAR)
    {
        // ε
    }
    else
    {
        int expected[] = {TOK_PLUS, TOK_RPAREN, TOK_DOLLAR};
        syntax_error(token, expected, 3);
    }
}

void T()
{
    if (token == TOK_ID || token == TOK_LPAREN)
    {
        F();
        if (!erro)
            T_();
    }
    else
    {
        int expected[] = {TOK_ID, TOK_LPAREN};
        syntax_error(token, expected, 2);
    }
}

void T_()
{
    if (token == TOK_STAR)
    {
        eat(TOK_STAR);
        if (!erro)
            F();
        if (!erro)
            T_();
    }
    else if (token == TOK_PLUS || token == TOK_RPAREN || token == TOK_DOLLAR)
    {
        // ε
    }
    else
    {
        int expected[] = {TOK_STAR, TOK_PLUS, TOK_RPAREN, TOK_DOLLAR};
        syntax_error(token, expected, 4);
    }
}

void F()
{
    if (token == TOK_ID)
    {
        eat(TOK_ID);
        if (!erro)
            P();
    }
    else if (token == TOK_LPAREN)
    {
        eat(TOK_LPAREN);
        if (!erro)
            E();
        if (!erro)
            eat(TOK_RPAREN);
        if (!erro)
            P();
    }
    else
    {
        int expected[] = {TOK_ID, TOK_LPAREN};
        syntax_error(token, expected, 2);
    }
}

void P()
{
    if (token == TOK_PLUSPLUS)
    {
        eat(TOK_PLUSPLUS);
    }
    else if (token == TOK_PLUS || token == TOK_STAR || token == TOK_RPAREN || token == TOK_DOLLAR)
    {
        // ε
    }
    else
    {
        int expected[] = {TOK_PLUSPLUS, TOK_PLUS, TOK_STAR, TOK_RPAREN, TOK_DOLLAR};
        syntax_error(token, expected, 5);
    }
}

int main()
{
    bool first_line = true;

    while (read_tokens_line())
    {
        if (!first_line)
        {
            printf("\n");
        }
        first_line = false;

        pos = 0;
        erro = false;

        advance(); // Inicializa token global
        S();

        if (!erro && token == TOK_UNKNOWN)
        {
            printf("CADEIA ACEITA");
        }
    }

    return 0;
}
