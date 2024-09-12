#include "utils.hpp"
#include "parser.tab.h"

int main() {
    yyparse();
    return 0;
}

