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
    string termo_valido;
    recon aux;
    int estado_atual = 1; // Estado inicial
    int ultimo_estado = 0;
    int ultimo_estado_valido = 0;
    int index = 0;
    while (index < (int)entrada.size())
    {
        for (int i = index; i <= (int)entrada.size(); ++i)
        {
            char c = entrada[i];
            ultimo_estado = estado_atual;
            if (mapa_tokens.find(ultimo_estado) != mapa_tokens.end())
            {
                // Se o último estado for final então salva
                ultimo_estado_valido = ultimo_estado;
                termo_valido = termo;
                // Salva também índice da string até ali
                // Para começar a leitura posterior a partir dali
                index = i;
            }
            estado_atual = automato.makeTransition(estado_atual, c);
            if (estado_atual == 0)
            {
                // Estado inválido, então verifica se algum token foi reconhecido
                if (mapa_tokens.find(ultimo_estado_valido) != mapa_tokens.end())
                {
                    // Token Válido dado pelo último estado final válido
                    aux.token = mapa_tokens[ultimo_estado_valido];
                }
                else
                {
                    // Token Inválido
                    termo += c;
                    ++index;
                    aux.token = "ERRO";
                }

                // Se for um espaço então não considera
                if (termo != " ")
                {
                    aux.cadeia = termo_valido;
                    lista_tokens.push_back(aux);
                }
                ultimo_estado_valido = 0;
                termo.clear();
                termo_valido.clear();
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
