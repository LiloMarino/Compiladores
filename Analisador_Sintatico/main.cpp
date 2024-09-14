#include "sintatico.tab.h"
#include <iostream>

int main()
{
    std::cout << std::endl << yyparse();
    return 0;
}
