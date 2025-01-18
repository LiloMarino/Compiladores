#ifndef REGALLOC_HPP
#define REGALLOC_HPP

#include "graph.hpp"
#include <memory>

class RegAlloc
{
private:
    std::unique_ptr<Graph> grafo;
    int K;

public:
    /**
     * @brief Construtor que inicializa a alocação de registradores com um ponteiro para um grafo e um valor de K.
     *        A propriedade do grafo é transferida para a classe.
     * @param grafo Ponteiro para o grafo de interferência a ser utilizado.
     * @param K O número de registradores disponíveis.
     */
    RegAlloc(std::unique_ptr<Graph> grafo, int K);


    /**
     * @brief Inicia o algoritmo de coloração dos grafos
     */
    void start();
};

#endif