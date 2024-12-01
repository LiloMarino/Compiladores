#include "sintatico.tab.h"
#include "dcmat.hpp"
#include <iostream>
#include <iomanip>
#include <string>
#include <cstdio>

extern FILE *yyin;
extern void yyrestart(FILE *input_file);
extern int yyparse();
extern void yylex_destroy();

int main()
{
    std::cout << std::fixed << std::setprecision(dcmat.settings.float_precision);
    while (true)
    {
        std::cout << "> ";
        std::string entrada;
        std::getline(std::cin, entrada);
        entrada += '\n';

        if (entrada.empty())
            continue;

        // Abre a string como um stream
        yyin = fmemopen(const_cast<char *>(entrada.data()), entrada.size(), "r");

        // Reinicializa o lexer
        yyrestart(yyin);

        // Executa o parsing
        yyparse();

        // Fecha o arquivo de entrada
        fclose(yyin);

        // Limpa qualquer estado residual do lexer
        yylex_destroy();
    }
    return 0;
}
