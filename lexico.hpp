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
    std::unordered_map<int, std::string> mapa_tokens;

public:
    AnalisadorLexico(AutomatoFinito &a);

    /**
     * @brief Associa o token ao estado final
     * @param token Token
     * @param estado_final Estado final do autômato
     */
    void addToken(const std::string &token, const int estado_final);

    /**
     * @brief Reconhece a string de entrada e transforma-a em tokens
     * @param entrada String de Entrada
     * @return Retorna a lista de tokens
     */
    std::list<recon> reconhecer(const std::string &entrada);
};

#endif