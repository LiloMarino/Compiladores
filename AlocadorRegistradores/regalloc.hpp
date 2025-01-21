#ifndef REGALLOC_HPP
#define REGALLOC_HPP

#include "graph.hpp"
#include <memory>
#include <unordered_map>
#include <stack>

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
private:
    /**
     * @brief Simplifica o grafo de interferência removendo os nós que não possuem dependências.
     * @param nodeStack Uma pilha de nós que precisam ser visitados.
     * @param currentK O número de registradores disponíveis.
     */
    void simplify(std::stack<GraphNode> &nodeStack, int currentK);

    /**
     * @brief Realiza a substituição de um nó por um registrador.
     * @param nodeStack Uma pilha de nós que precisam ser visitados.
     */
    void spill(std::stack<GraphNode> &nodeStack);

    /**
     * @brief Seleciona os nós que precisam ser visitados.
     * @param nodeStack Uma pilha de nós que precisam ser visitados.
     */
    void select(std::stack<GraphNode> &nodeStack);
};

#endif