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
    recon aux;
    int estado_atual = 1; // Estado inicial
    int ultimo_estado = 0;
    int inicio_string = 0;
    while (inicio_string < (int)entrada.size())
    {
        for (int i = inicio_string; i <= (int)entrada.size(); ++i)
        {
            char c = entrada[i];
            ultimo_estado = estado_atual;
            estado_atual = automato.makeTransition(estado_atual, c);
            if (estado_atual == 0)
            {
                // Estado inválido, então verifica se algum token foi reconhecido
                if (mapa_tokens.find(ultimo_estado) != mapa_tokens.end())
                {
                    // Token Válido
                    aux.token = mapa_tokens[ultimo_estado];
                    inicio_string = i;
                }
                else
                {
                    // Token Inválido
                    termo += c;
                    ++inicio_string;
                    aux.token = "ERRO";
                }

                // Se for um espaço então não considera
                if (termo != " ")
                {
                    aux.cadeia = termo;
                    lista_tokens.push_back(aux);
                }
                termo.clear();
                break;
            }
            else
            {
                termo += c;
            }
        }
        estado_atual = 1; // Reseta para o estado inicial
    }
    return lista_tokens;
}
