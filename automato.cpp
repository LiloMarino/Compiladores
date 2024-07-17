#include "automato.hpp"
#include <cstdlib>
#include <cstdio>
#include <string>
#include <stdexcept>

#define ASCII_SIZE 256

using namespace std;

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

void Automato::addTransitions(const int estado_inicial, const int estado_final, const string &transitions)
{
    if ((estado_inicial >= num_estados || estado_inicial < 0) ||
        (estado_final >= num_estados || estado_final < 0))
    {
        throw length_error("Estados Inválidos");
    }

    bool intervalo = false;
    char inicio, fim;
    for (const char atual : transitions)
    {
        if (atual == ',')
        {
            // Cria a transição
            if (intervalo)
            {
                for (char i = inicio; i <= fim; ++i)
                {
                    matriz[estado_inicial][(int)i] = estado_final;
                }
                intervalo = false;
            }
            else
            {
                matriz[estado_inicial][(int)fim] = estado_final;
            }
        }
        else if (atual == '-')
        {
            // Salva o início e determina a leitura de um intervalo
            inicio = fim;
            intervalo = true;
        }
        fim = atual;
    }
}

int Automato::makeTransition(const int estado_atual, const char letra)
{
    if (estado_atual >= num_estados || estado_atual < 0)
    {
        throw length_error("Estado Inválido");
    }
    int estado_destino = matriz[estado_atual][(int)letra];
    return estado_destino;
}

int Automato::getNumEstados()
{
    return num_estados;
}
