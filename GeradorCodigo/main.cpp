#include "mips.hpp"

extern int yyparse();

int main()
{
    MIPS::initialize();
    yyparse();
}
