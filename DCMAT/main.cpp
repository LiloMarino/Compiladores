#include "sintatico.tab.h"
#include "dcmat.hpp"
#include <iostream>

extern int yyparse();
extern FILE *yyin;

int main()
{
    init();
    while (true)
    {
        std::cout << "> ";
        std::string entrada;
        std::getline(std::cin, entrada);

        yyin = fmemopen(const_cast<char *>(entrada.data()), entrada.size(), "r");
        yyparse();
    }
    return 0;
}
