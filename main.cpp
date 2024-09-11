#include <iostream>
#include "parser.tab.h"

int main() {
    // Inicializa a análise sintática
    if (yyparse() == 0) {
        std::cout << "EXPRESSAO CORRETA" << std::endl;
    } else {
        std::cerr << "A expressao terminou de forma inesperada." << std::endl;
    }
    return 0;
}
