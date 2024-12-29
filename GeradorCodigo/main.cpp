#include "mips.hpp"

extern int yyparse();
extern int yydebug;

int main()
{
    yydebug = 1;
    MIPS::initialize();
    yyparse();
}
