#include <iostream>
#include "parser.tab.h"

int main() {
    // Inicializa a análise sintática
    if (yyparse() == 0) {
        std::cout << "Análise concluída com sucesso!" << std::endl;
    } else {
        std::cerr << "Erro durante a análise." << std::endl;
    }
    return 0;
}
