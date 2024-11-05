#include "sintatico.tab.h"
#include <iostream>

extern bool firstLine;

int main()
{
    yyparse();
    if (!firstLine)
    {
        std::cout<< std::endl;
    }
    std::cout << "SUCCESSFUL COMPILATION.";
    return 0;
}
