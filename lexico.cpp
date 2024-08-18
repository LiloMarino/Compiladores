#include "lexico.hpp"
#include <stdexcept>

using namespace std;

AnalisadorLexico::AnalisadorLexico(Automato &a) : automato(a)
{
}

void AnalisadorLexico::addToken(const string &token, const int estado_final)
{
    if (estado_final >= automato.getNumEstados() || estado_final < 0)
    {
        throw length_error("Estado Inválido");
    }
    mapa_tokens[estado_final] = token;
}

list<recon> AnalisadorLexico::reconhecer(const string &entrada)
{
    list<recon> lista_tokens;
    string termo;
    int estado_atual = 1; // Estado inicial
    int ultimo_estado_valido = 0;
    size_t index = 0;

    while (index < entrada.size())
    {
        termo.clear();
        ultimo_estado_valido = 0;

        for (size_t i = index; i < entrada.size(); ++i)
        {
            char c = entrada[i];
            int novo_estado = automato.makeTransition(estado_atual, c);
            if (novo_estado == 0)
            {
                // Estado inválido, então verifica se algum token foi reconhecido
                if (ultimo_estado_valido != 0)
                {
                    // Token Válido dado pelo último estado final válido
                    lista_tokens.push_back({mapa_tokens[ultimo_estado_valido], termo});
                    index = i;
                }
                else
                {
                    // Token Inválido
                    lista_tokens.push_back({"ERRO", termo + c});
                    ++index;
                }
                break;
            }

            termo += c;
            estado_atual = novo_estado;

            // Verifica se o estado atual é final
            if (mapa_tokens.find(estado_atual) != mapa_tokens.end())
            {
                // O estado atual é final então salva
                ultimo_estado_valido = estado_atual;
            }

            // Se chegou ao final sem estado inválido, processa o termo restante
            if (i == entrada.size() - 1 && ultimo_estado_valido != 0)
            {
                lista_tokens.push_back({mapa_tokens[ultimo_estado_valido], termo});
                index = i + 1;
            }
        }
        estado_atual = 1; // Reseta para o estado inicial
    }
    return lista_tokens;
}
