#ifndef LEXICO_HPP
#define LEXICO_HPP

#include "automato_finito.hpp"
#include <unordered_map>
#include <list>

struct recon
{
    std::string token;
    std::string cadeia;
};

class AnalisadorLexico
{
private:
    AutomatoFinito &automato;
    std::unordered_set<char> ignore_symbols;

public:
    AnalisadorLexico(AutomatoFinito &a);

    /**
     * @brief Reconhece a string de entrada e transforma-a em tokens
     * @param entrada String de Entrada
     * @return Retorna a lista de tokens
     */
    std::list<recon> reconhecer(const std::string &entrada);

    /**
     * @brief Adiciona um símbolo a ser ignorado
     * @param symbol Símbolo a ser ignorado
     */
    void addIgnoreSymbol(char symbol);

    /**
     * @brief Obtém a referência ao autômato finito.
     * @return Referência ao autômato finito.
     */
    AutomatoFinito &getAutomato() const;

    /**
     * @brief Define a referência ao autômato finito.
     * @param a Nova referência ao autômato finito.
     */
    void setAutomato(AutomatoFinito &a);
};

#endif