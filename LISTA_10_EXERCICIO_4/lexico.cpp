#include "lexico.hpp"
#include <stdexcept>

using namespace std;

AnalisadorLexico::AnalisadorLexico(AutomatoFinito &a) : automato(a)
{
}

list<LexicalGroup> AnalisadorLexico::reconhecer(const string &entrada)
{
    list<LexicalGroup> lista_tokens;
    string termo;
    int estado_atual = 1; // Estado inicial
    int ultimo_estado_valido = 0;
    size_t index = 0;

    while (index < entrada.size())
    {
        termo.clear();
        ultimo_estado_valido = 0;
        estado_atual = 1; // Reseta para o estado inicial para cada novo token

        for (size_t i = index; i < entrada.size(); ++i)
        {
            char c = entrada[i];

            // Se o caractere atual é um símbolo a ser ignorado
            if (ignore_symbols.find(c) != ignore_symbols.end())
            {
                if (!termo.empty() && ultimo_estado_valido != 0)
                {
                    // Adiciona o token atual à lista antes de ignorar o símbolo
                    lista_tokens.push_back({this->automato.tokens.getTokenByFinalState(ultimo_estado_valido), termo});
                }
                // Avança o index para começar um novo token depois do símbolo ignorado
                index = i + 1;
                break;
            }

            int novo_estado = automato.makeTransition(estado_atual, c);
            if (novo_estado == 0)
            {
                // Estado inválido, então verifica se algum token foi reconhecido
                if (ultimo_estado_valido != 0)
                {
                    // Token válido dado pelo último estado final válido
                    lista_tokens.push_back({this->automato.tokens.getTokenByFinalState(ultimo_estado_valido), termo});
                    index = i;
                }
                else
                {
                    // Token inválido
                    throw LexicalError(termo + c);
                    // lista_tokens.push_back({"ERRO", termo + c});
                    // index = i + 1;
                }
                break;
            }

            termo += c;
            estado_atual = novo_estado;

            // Verifica se o estado atual é final
            if (this->automato.tokens.isFinalState(estado_atual))
            {
                // O estado atual é final, então salva
                ultimo_estado_valido = estado_atual;
            }

            // Se chegou ao final sem estado inválido, processa o termo restante
            if (i == entrada.size() - 1 && ultimo_estado_valido != 0)
            {
                lista_tokens.push_back({this->automato.tokens.getTokenByFinalState(ultimo_estado_valido), termo});
                index = i + 1;
            }
        }

        // Caso não tenha encontrado um símbolo ignorado e o loop interno terminou
        if (index == entrada.size())
        {
            break;
        }
    }
    return lista_tokens;
}

void AnalisadorLexico::addIgnoreSymbol(char symbol)
{
    ignore_symbols.insert(symbol);
}

AutomatoFinito &AnalisadorLexico::getAutomato() const
{
    return automato;
}

void AnalisadorLexico::setAutomato(AutomatoFinito &a)
{
    automato = a;
}

AnalisadorLexico::LexicalError::LexicalError(const std::string &token)
{
    std::string erro;
    erro += "ERRO LEXICO: ";
    erro += token;
    this->mensagem = std::move(erro);
}

const char *AnalisadorLexico::LexicalError::what() const noexcept
{
    return mensagem.c_str();
}
