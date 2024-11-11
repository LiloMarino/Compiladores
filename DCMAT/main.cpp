#include "sintatico.tab.h"
#include <iostream>

extern int yyparse();
extern FILE *yyin;

int main() {
    while (true) {
        std::cout << "> ";
        std::string entrada;
        std::getline(std::cin, entrada);

        if (entrada == "sair") {
            break;
        }

        yyin = fmemopen(entrada.data(), entrada.size(), "r");

        // Chama o parser
        if (yyparse() == 0) {
            std::cout << "Expressão avaliada com sucesso!\n";
        } else {
            std::cerr << "Erro durante o parsing!\n";
        }
    }

    return 0;
}
