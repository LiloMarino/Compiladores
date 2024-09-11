#include "utils.hpp"
#include <iostream>
#include <vector>
extern int column;
extern std::vector<std::string> invalid_tokens;

void log_error(const char* msg) {
    std::cerr << "Erro de sintaxe na coluna [" << column << "]: " << msg << std::endl;
}

void log_lexical_error() {
    std::cout << "Caractere(s) invalido(s) -> [";
    for (size_t i = 0; i < invalid_tokens.size(); ++i) {
        std::cout << invalid_tokens[i];
        if (i < invalid_tokens.size() - 1) {
            std::cout << ",";
        }
    }
    std::cout << "]" << std::endl;
    invalid_tokens.clear();
}