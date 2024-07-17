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
    int estado_atual = 1; // Estado inicial
    int ultimo_estado = 0;
    for (const char c : entrada)
    {
        ultimo_estado = estado_atual;
        estado_atual = automato.makeTransition(estado_atual, c);
        if (estado_atual == 0)
        {
            // Estado inválido, então verifica se algum token foi reconhecido
            // Procura no mapa de tokens verificando se o último estado é final
            if (mapa_tokens.find(ultimo_estado) != mapa_tokens.end())
            {
                lista_tokens.push_back(mapa_tokens[ultimo_estado]);
            }
            else
            {
                // Não é um estado final, então reconhece um ERRO
                lista_tokens.push_back("ERRO");
            }
            
        }
    }
    return lista_tokens;
}
