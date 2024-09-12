#include "parser.tab.h"

int main()
{
    while (yyparse() == 0)
    {
    }
    return 0;
}
