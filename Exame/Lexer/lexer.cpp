#include <iostream>
#include <string>
#include <cctype>

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

// Retorna o próximo token e avança a posição
TokenType next_token(const std::string &line, size_t &pos)
{
    if (line.compare(pos, 2, "id") == 0)
    {
        pos += 2;
        return TOK_ID;
    }

    if (line.compare(pos, 2, "++") == 0)
    {
        pos += 2;
        return TOK_PLUSPLUS;
    }

    switch (line[pos])
    {
    case '+':
        ++pos;
        return TOK_PLUS;
    case '*':
        ++pos;
        return TOK_STAR;
    case '(':
        ++pos;
        return TOK_LPAREN;
    case ')':
        ++pos;
        return TOK_RPAREN;
    case '$':
        ++pos;
        return TOK_DOLLAR;
    default:
        ++pos;
        return TOK_UNKNOWN;
    }
}

int main()
{
    std::string line;
    
    bool first_line = true;
    while (std::getline(std::cin, line))
    {   
        if (!first_line)
        {
            std::cout << '\n'; // CORRETO
        }
        first_line = false;
        for (size_t pos = 0; pos < line.size();)
        {
            if (isspace(line[pos]))
            {
                ++pos;
                continue;
            }

            TokenType tok = next_token(line, pos);
            if (tok != TOK_UNKNOWN)
            {
                std::cout << tok;
            }
        }
        // std::cout << '\n'; ERRADO
    }

    return 0;
}