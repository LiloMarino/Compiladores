#include "lexico.hpp"
#include <stdexcept>

using namespace std;

AnalisadorLexico::AnalisadorLexico(AutomatoFinito &a) : automato(a)
{
}

list<LexicalGroup> AnalisadorLexico::reconhecer(const string &entrada)
{
    column = 1;
    ++line;
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

        for (size_t i = index; i < entrada.size(); ++i, ++column)
        {
            char c = entrada[i];

            int novo_estado = automato.makeTransition(estado_atual, c);
            if (novo_estado == 0)
            {
                // Estado inválido, então verifica se algum token foi reconhecido
                if (ultimo_estado_valido != 0)
                {
                    // Token válido dado pelo último estado final válido
                    verifyToken(ultimo_estado_valido, lista_tokens, termo);
                    index = i;
                }
                else
                {
                    // Token inválido
                    LexicalGroup error;
                    error.cadeia += c;
                    error.token_column = this->column;
                    error.token_line = this->line;
                    throw LexicalError(error);
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
                verifyToken(ultimo_estado_valido, lista_tokens, termo);
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

void AnalisadorLexico::verifyToken(int ultimo_estado_valido, std::list<LexicalGroup> &lista_tokens, std::string &termo)
{
    std::string token = this->automato.tokens.getTokenByFinalState(ultimo_estado_valido);
    // Verifica se não é um token ignorado
    if (ignore_tokens.find(token) == ignore_tokens.end())
    {
        // Não é um token ignorado, então verifica se é um comentário multilinha
        if (token == multiline_tokens.first)
        {
            multiline_comment = true;
        }
        else if (token == multiline_tokens.second)
        {
            multiline_comment = false;
        }
        else if (!multiline_comment)
        {
            // Token válido então adiciona na lista
            lista_tokens.push_back({std::move(token), termo});
        }
    }
}

void AnalisadorLexico::addIgnoreToken(const std::string &token)
{
    ignore_tokens.insert(token);
}

AutomatoFinito &AnalisadorLexico::getAutomato() const
{
    return automato;
}

void AnalisadorLexico::setAutomato(AutomatoFinito &a)
{
    automato = a;
}

void AnalisadorLexico::setMultilineComment(const std::string &token_start, const std::string &token_end)
{
    multiline_tokens.first = token_start;
    multiline_tokens.second = token_end;
}

AnalisadorLexico::LexicalError::LexicalError(const LexicalGroup &token)
{
    std::string erro;
    erro += "ERRO LEXICO. Linha: " + to_string(token.token_line) + " Coluna: " + to_string(token.token_column) + " -> '" + token.cadeia + "'";
    this->mensagem = std::move(erro);
}

const char *AnalisadorLexico::LexicalError::what() const noexcept
{
    return mensagem.c_str();
}
