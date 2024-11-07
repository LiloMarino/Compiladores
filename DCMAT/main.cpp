#include "sintatico.tab.h"
#include <iostream>

// Declaração de yylex
extern int yylex();
extern const char* getTokenName(int token);

int main()
{
    int token;
    while ((token = yylex()) != 0) {
        // Imprime o nome do token usando o índice de token para acessar yytname
        std::cout << "Token: " << getTokenName(token) << std::endl;
    }

    return 0;
}
