#include "automato_finito.hpp"
#include <string>
#include <stdexcept>
#include <iostream>
#include <iomanip>

#define ASCII_SIZE 256

using namespace std;

AutomatoFinito::AutomatoFinito() : num_estados(0), matriz(nullptr), deterministico(false), afnd(nullptr)
{
    afnd = new GenericAutomata();
}

AutomatoFinito::AutomatoFinito(int num_estados) : matriz(nullptr), afnd(nullptr)
{
    num_estados++; // Adiciona o estado de erro
    this->num_estados = num_estados;
    matriz = new int *[num_estados]();
    for (int i = 0; i < num_estados; ++i)
    {
        matriz[i] = new int[ASCII_SIZE]();
    }
    deterministico = true;
}

AutomatoFinito::~AutomatoFinito()
{
    if (matriz != nullptr)
    {
        for (int i = 0; i < num_estados; ++i)
        {
            delete[] matriz[i];
        }
        delete[] matriz;
    }
    if (afnd != nullptr)
    {
        delete afnd;
    }
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

void AutomatoFinito::addRegularExpression(const std::string &re)
{
    if (this->deterministico)
    {
        throw logic_error("O Autômato é determinístico");
    }
    this->afnd->addRegularExpression(re);
}

int AutomatoFinito::makeTransition(const int estado_atual, const char letra)
{
    if (!this->deterministico)
    {
        throw logic_error("O Autômato não é determinístico");
    }
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

void AutomatoFinito::printTransitionTable()
{
    cout << setw(15) << "Estado\\Input";
    for (int i = 0; i < ASCII_SIZE; ++i)
    {
        string rep;
        if (i < 32 || i == 127 || i >= 128)
        {
            // Para caracteres não visíveis ou com problemas na impressão
            rep = "ASCII " + to_string(i);
        }
        else
        {
            // Para caracteres visíveis no intervalo ASCII padrão (32 a 126)
            rep = string(1, static_cast<char>(i));
        }
        cout << setw(15) << rep;
    }
    cout << endl;
    if (deterministico)
    {
        // Printa a tabela do AFD
        for (int i = 0; i < num_estados; ++i)
        {
            cout << setw(15) << "q" + to_string(i);
            for (int j = 0; j < ASCII_SIZE; ++j)
            {
                cout << setw(15) << matriz[i][j];
            }
            cout << endl;
        }
    }
    else
    {
        // Printa a tabela do AFND
        for (auto &state : *afnd)
        {
            cout << setw(15) << "q" + to_string(state.getEstado());
            for (int j = 0; j < ASCII_SIZE; ++j)
            {
                char input = static_cast<char>(j);
                bool transition_found = false;
                string trans_str = "{";

                // Itera sobre as transições para encontrar as correspondentes ao input
                for (const auto &transition : state.getTransitions())
                {
                    if (transition.entrada == input)
                    {
                        transition_found = true;
                        trans_str += "q" + to_string(transition.estado_destino->getEstado()) + ",";
                    }
                }

                if (transition_found)
                {
                    if (trans_str.back() == ',')
                    {
                        trans_str.pop_back(); // Remove a última vírgula
                    }
                    trans_str += "}";
                    cout << setw(15) << trans_str;
                }
                else
                {
                    cout << setw(15) << "-";
                }
            }
            cout << endl;
        }
    }
}

bool AutomatoFinito::operator==(const AutomatoFinito &outro) const
{
    if (num_estados != outro.num_estados)
    {
        return false;
    }
    if (!deterministico || !outro.deterministico)
    {
        // Só compara se os 2 forem determinísticos
        return false;
    }
    for (int i = 0; i < num_estados; ++i)
    {
        for (int j = 0; j < ASCII_SIZE; ++j)
        {
            if (matriz[i][j] != outro.matriz[i][j])
            {
                return false;
            }
        }
    }
    return true;
}
