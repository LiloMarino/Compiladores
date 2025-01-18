#ifndef REGALLOC_HPP
#define REGALLOC_HPP

#include "graph.hpp"
#include <memory>
#include <unordered_map>

class RegAlloc
{
private:
    std::unique_ptr<Graph> graph;
    std::unordered_map<int, int> colorMap;
    int K;

public:
    /**
     * @brief Construtor que inicializa a alocação de registradores com um ponteiro para um grafo e um valor de K.
     *        A propriedade do grafo é transferida para a classe.
     * @param graph Ponteiro para o grafo de interferência a ser utilizado.
     * @param K O número de registradores disponíveis.
     */
    RegAlloc(std::unique_ptr<Graph> graph, int K);

    /**
     * @brief Inicia o algoritmo de coloração dos grafos
     */
    void start();
};

#endif