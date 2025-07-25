#include <stdio.h>
#include <string.h>
#include <ctype.h>

typedef enum
{
    TOK_ID = 0,   // "id"
    TOK_PLUS,     // "+"
    TOK_STAR,     // "*"
    TOK_PLUSPLUS, // "++"
    TOK_LPAREN,   // "("
    TOK_RPAREN,   // ")"
    TOK_DOLLAR,   // "$"
    TOK_UNKNOWN
} TokenType;

// Retorna o próximo token e avança a posição
TokenType next_token(const char *line, size_t *pos)
{
    if (strncmp(&line[*pos], "id", 2) == 0)
    {
        *pos += 2;
        return TOK_ID;
    }

    if (strncmp(&line[*pos], "++", 2) == 0)
    {
        *pos += 2;
        return TOK_PLUSPLUS;
    }

    switch (line[*pos])
    {
    case '+':
        (*pos)++;
        return TOK_PLUS;
    case '*':
        (*pos)++;
        return TOK_STAR;
    case '(':
        (*pos)++;
        return TOK_LPAREN;
    case ')':
        (*pos)++;
        return TOK_RPAREN;
    case '$':
        (*pos)++;
        return TOK_DOLLAR;
    default:
        (*pos)++;
        return TOK_UNKNOWN;
    }
}

int main()
{
    char line[1024];
    int first_line = 1;

    while (fgets(line, sizeof(line), stdin))
    {
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n')
        {
            line[len - 1] = '\0'; // remove newline
        }

        if (!first_line)
        {
            printf('\n'); // CORRETO
        }
        first_line = 0;

        for (size_t pos = 0; pos < strlen(line);)
        {
            if (isspace((unsigned char)line[pos]))
            {
                ++pos;
                continue;
            }

            TokenType tok = next_token(line, &pos);
            if (tok != TOK_UNKNOWN)
            {
                printf("%d", tok);
            }
        }
        // putchar('\n'); ERRADO
    }

    return 0;
}
