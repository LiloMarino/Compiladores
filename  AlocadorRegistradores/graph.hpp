#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <vector>
#include <list>
#include <unordered_map>
#include <iostream>

class Graph
{
private:
    std::unordered_map<int, std::list<int>> adjacencyList;

public:
    /**
     * @brief Adiciona uma aresta do vértice u ao vértice v.
     * @param u O vértice de origem.
     * @param v O vértice de destino.
     */
    void addEdge(int u, int v);

    /**
     * @brief Adiciona uma lista de adjacência ao grafo.
     * @param u O vértice ao qual a lista de adjacência será associada.
     * @param adjList A lista de vértices adjacentes ao vértice u.
     */
    void addAdjacencyList(int u, const std::list<int> &adjList);

    /**
     * @brief Remove uma aresta do vértice u ao vértice v.
     * @param u O vértice de origem.
     * @param v O vértice de destino.
     */
    void removeEdge(int u, int v);

    /**
     * @brief Obtém a lista de adjacência de um vértice.
     * @param u O vértice cuja lista de adjacência será obtida.
     * @return Uma referência à lista de vértices adjacentes.
     */
    const std::list<int> &getAdjacencyList(int u) const;

    /**
     * @brief Exibe o grafo como listas de adjacência.
     */
    void display() const;
};

#endif