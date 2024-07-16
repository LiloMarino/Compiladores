#include "automato.hpp"
#include <cstdlib>
#include <cstdio>
#include <string>

#define ASCII_SIZE 256

Automato::Automato(int num_estados)
{
    matriz = new int *[num_estados];
    for (int i = 0; i < num_estados; ++i)
    {
        matriz[i] = new int[ASCII_SIZE];
    }
}

Automato::~Automato()
{
    for (int i = 0; i < num_estados; ++i)
    {
        delete[] matriz[i];
    }
    delete[] matriz;
}

void Automato::addTransition(const int estado_inicial, const int estado_final, const std::string &transitions)
{
}
