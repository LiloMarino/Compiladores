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
    bool multiline_comment = false;
    std::pair<std::string, std::string> multiline_tokens;

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
     * @brief Define os tokens de um comentário multilinha
     * @param token_start Token de início do comentário multilinha
     * @param token_end Token de fim do comentário multilinha
     */
    void setMultilineComment(const std::string &token_start, const std::string &token_end);

    /**
     * @brief Exceção do AnalisadorLéxico
     */
    class LexicalError : public std::exception
    {
    private:
        std::string mensagem;

    public:
        explicit LexicalError(const std::string &token);

        virtual const char *what() const noexcept override;
    };
private:
    /**
     * @brief Faz as verificações necessárias de comentário e adiciona a lista de tokens caso não seja um
     * @param ultimo_estado_valido Último estado válido
     * @param lista_tokens Lista de tokens
     * @param termo Termo reconhecido
     */
    void verifyComment(int ultimo_estado_valido, std::list<LexicalGroup> &lista_tokens, std::string &termo);
};

#endif