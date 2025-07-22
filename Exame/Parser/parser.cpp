#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <set>
#include <algorithm>

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
std::vector<int> tokens;
size_t pos = 0;
bool erro = false;

// Função para converter token para string
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
bool read_tokens_line(std::vector<int> &result)
{
    std::string line;
    if (!std::getline(std::cin, line))
    {
        return false; // EOF
    }

    result.clear();
    for (char c : line)
    {
        if (c >= '0' && c <= '9')
        {
            result.push_back(c - '0');
        }
    }

    return true; // Leitura feita com sucesso, mesmo que vazia
}

// Funções auxiliares
int lookahead()
{
    if (pos < tokens.size())
    {
        return tokens[pos];
    }
    return -1;
}

void advance()
{
    if (pos < tokens.size())
    {
        pos++;
    }
}

void syntax_error(int got, std::set<int> expected)
{
    erro = true;
    std::cout << "ERRO SINTATICO EM: " << token_to_string(got) << " ESPERADO: ";
    bool first = true;
    for (int e : expected)
    {
        if (!first)
            std::cout << ", ";
        std::cout << token_to_string(e);
        first = false;
    }
}

void eat(int t)
{
    if (lookahead() == t)
    {
        advance();
    }
    else
    {
        syntax_error(lookahead(), {t});
    }
}

// Produções da gramática
void S();
void E();
void E_();
void T();
void T_();
void F();
void P();

void S()
{
    switch (lookahead())
    {
    case TOK_ID:
    case TOK_LPAREN:
        E();
        if (!erro)
            eat(TOK_DOLLAR);
        break;
    default:
        syntax_error(lookahead(), {TOK_ID, TOK_LPAREN});
    }
}

void E()
{
    switch (lookahead())
    {
    case TOK_ID:
    case TOK_LPAREN:
        T();
        if (!erro)
            E_();
        break;
    default:
        syntax_error(lookahead(), {TOK_ID, TOK_LPAREN});
    }
}

void E_()
{
    switch (lookahead())
    {
    case TOK_PLUS:
        eat(TOK_PLUS);
        if (!erro)
            T();
        if (!erro)
            E_();
        break;
    case TOK_RPAREN:
    case TOK_DOLLAR:
        // epsilon
        break;
    default:
        syntax_error(lookahead(), {TOK_PLUS, TOK_RPAREN, TOK_DOLLAR});
    }
}

void T()
{
    switch (lookahead())
    {
    case TOK_ID:
    case TOK_LPAREN:
        F();
        if (!erro)
            T_();
        break;
    default:
        syntax_error(lookahead(), {TOK_ID, TOK_LPAREN});
    }
}

void T_()
{
    switch (lookahead())
    {
    case TOK_STAR:
        eat(TOK_STAR);
        if (!erro)
            F();
        if (!erro)
            T_();
        break;
    case TOK_PLUS:
    case TOK_RPAREN:
    case TOK_DOLLAR:
        // epsilon
        break;
    default:
        syntax_error(lookahead(), {TOK_STAR, TOK_PLUS, TOK_RPAREN, TOK_DOLLAR});
    }
}

void F()
{
    switch (lookahead())
    {
    case TOK_ID:
        eat(TOK_ID);
        if (!erro)
            P();
        break;
    case TOK_LPAREN:
        eat(TOK_LPAREN);
        if (!erro)
            E();
        if (!erro)
            eat(TOK_RPAREN);
        if (!erro)
            P();
        break;
    default:
        syntax_error(lookahead(), {TOK_ID, TOK_LPAREN});
    }
}

void P()
{
    switch (lookahead())
    {
    case TOK_PLUSPLUS:
        eat(TOK_PLUSPLUS);
        break;
    case TOK_PLUS:
    case TOK_STAR:
    case TOK_RPAREN:
    case TOK_DOLLAR:
        // epsilon
        break;
    default:
        syntax_error(lookahead(), {TOK_PLUSPLUS, TOK_PLUS, TOK_STAR, TOK_RPAREN, TOK_DOLLAR});
    }
}

int main()
{
    bool first_line = true;
    std::vector<int> linha;

    while (true)
    {
        if (!read_tokens_line(linha))
        {
            break;
        }

        if (!first_line)
        {
            std::cout << '\n';
        }
        first_line = false;

        tokens = linha;
        pos = 0;
        erro = false;

        S();

        if (!erro && pos == tokens.size())
        {
            std::cout << "CADEIA ACEITA";
        }
    }

    return 0;
}
