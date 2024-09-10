#include "utils.hpp"
#include <iostream>
extern int column;

void log_error(const char* msg) {
    std::cerr << "Erro de sintaxe na coluna [" << column << "]: " << msg << std::endl;
}
