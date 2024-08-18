#include "automato_finito.hpp"
#include <cstdlib>
#include <cstdio>
#include <string>
#include <stdexcept>

#define ASCII_SIZE 256

using namespace std;

AutomatoFinito::AutomatoFinito(int num_estados)
{
    num_estados++; // Adiciona o estado de erro
    this->num_estados = num_estados;
    matriz = new int *[num_estados]();
    for (int i = 0; i < num_estados; ++i)
    {
        matriz[i] = new int[ASCII_SIZE]();
    }
}

AutomatoFinito::~AutomatoFinito()
{
    for (int i = 0; i < num_estados; ++i)
    {
        delete[] matriz[i];
    }
    delete[] matriz;
}

void AutomatoFinito::addTransitions(const int estado_inicial, const int estado_final, const string &transitions)
{
    if ((estado_inicial >= num_estados || estado_inicial < 0) ||
        (estado_final >= num_estados || estado_final < 0))
    {
        throw length_error("Estados Inválidos");
    }

    if (transitions == "other")
    {
        // Qualquer outra transição não definida (0)
        for (int i = 0; i < ASCII_SIZE; ++i)
        {
            if (matriz[estado_inicial][i] == 0)
            {
                matriz[estado_inicial][i] = estado_final;
            }
        }
    }
    else
    {
        bool intervalo = false;
        char inicio, fim;
        for (size_t i = 0; i < transitions.length(); i++)
        {
            const char atual = transitions[i];
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
            else if (atual == '-' && i + 1 < transitions.length())
            {
                // Salva o início e determina a leitura de um intervalo
                inicio = fim;
                intervalo = true;
            }
            fim = atual;
        }
        // Cria a última transição
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
}

int AutomatoFinito::makeTransition(const int estado_atual, const char letra)
{
    if (estado_atual >= num_estados || estado_atual < 0)
    {
        throw length_error("Estado Inválido");
    }
    int estado_destino = matriz[estado_atual][(int)letra];
    return estado_destino;
}

int AutomatoFinito::getNumEstados()
{
    return num_estados;
}
