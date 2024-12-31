#include "mips.hpp"

extern int yyparse();

int main()
{
    MIPS::freeAllRegisters();
    yyparse();
}
