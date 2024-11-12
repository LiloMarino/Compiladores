#include "sintatico.tab.h"
#include <iostream>

extern int yyparse();
extern FILE *yyin;

int main()
{
    while (true)
    {
        std::cout << "> ";
        std::string entrada;
        std::getline(std::cin, entrada);

        yyin = fmemopen(entrada.data(), entrada.size(), "r");
        yyparse();
    }
    return 0;
}
