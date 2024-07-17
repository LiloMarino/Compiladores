#ifndef RECONHECEDOR_HPP
#define RECONHECEDOR_HPP

#include "automato.hpp"
#include <map>
#include <list>

struct recon
{
    std::string cadeia;
    std::string token;
};

class Reconhecedor
{
private:
    Automato &automato;
    std::map<int, std::string> mapa_tokens;

public:
    Reconhecedor(Automato &a);

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