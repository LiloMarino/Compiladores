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

/**
 * @brief Classe de Analisador Léxico
 * @author Murilo Marino
 */
class AnalisadorLexico
{
private:
    AutomatoFinito &automato;
    std::unordered_set<std::string> ignore_tokens;
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
     * @brief Adiciona um token a ser ignorado
     * @param token Token a ser ignorado
     */
    void addIgnoreToken(const std::string &token);

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
     * @brief Faz as verificações necessárias para o token e adiciona a lista de tokens caso não seja um comentário ou
     * um token ignorado
     * @param ultimo_estado_valido Último estado válido
     * @param lista_tokens Lista de tokens
     * @param termo Termo reconhecido
     */
    void verifyToken(int ultimo_estado_valido, std::list<LexicalGroup> &lista_tokens, std::string &termo);
};

#endif