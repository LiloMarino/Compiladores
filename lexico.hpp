#ifndef LEXICO_HPP
#define LEXICO_HPP

#include "automato_finito.hpp"
#include <unordered_map>
#include <list>

struct LexicalGroup
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
    std::list<LexicalGroup> reconhecer(const std::string &entrada);

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

    /**
     * @brief Exceção do AnalisadorLéxico
     */
    class LexicalError : public std::exception
    {
    private:
        std::string mensagem;

    public:
        explicit LexicalError(const std::string &msg) : mensagem(msg) {}

        virtual const char *what() const noexcept override;
    };
};

#endif