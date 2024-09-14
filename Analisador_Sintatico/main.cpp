#include "sintatico.tab.h"
#include <iostream>

int main()
{
    yyparse();
    std::cout << "SUCCESSFUL COMPILATION.";
    return 0;
}
