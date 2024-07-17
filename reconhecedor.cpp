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

list<recon> Reconhecedor::reconhecer(const string &entrada)
{
    list<recon> lista_tokens;
    int estado_atual = 1; // Estado inicial
    int ultimo_estado = 0;
    string termo;
    recon aux;

    for (const char c : entrada)
    {
        termo += c;
        ultimo_estado = estado_atual;
        estado_atual = automato.makeTransition(estado_atual, c);

        if (estado_atual == 0)
        {
            // Estado inválido, então verifica se algum token foi reconhecido
            if (mapa_tokens.find(ultimo_estado) != mapa_tokens.end())
            {
                aux.token = mapa_tokens[ultimo_estado];
            }
            else
            {
                aux.token = "ERRO";
            }
            aux.cadeia = termo;
            lista_tokens.push_back(aux);
            termo.clear();
        }
    }

    return lista_tokens;
}
