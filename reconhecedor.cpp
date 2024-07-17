#include "reconhecedor.hpp"
#include <stdexcept>

using namespace std;

Reconhecedor::Reconhecedor(Automato &a) : automato(a)
{
}

void Reconhecedor::addToken(const string &token, const int estado_final)
{
    if (estado_final >= automato.getNumEstados() || estado_final < 0)
    {
        throw length_error("Estado Inválido");
    }
    mapa_tokens[estado_final] = token;
}

list<string> Reconhecedor::reconhecer(const string &entrada)
{
    list<string> lista_tokens;
    int estado = 1; // Estado inicial
    for (int i = 0; i < entrada.size(); i++)
    {
        char c = entrada[i];
        if (c == '\n')
        {
            c = ' ';
        }
    }
}
