#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <vector>
#include <list>
#include <unordered_map>
#include <iostream>


struct GraphNode
{
    int virtualRegister;
    std::list<int> adjacencyList;
};


class Graph
{
private:
    std::unordered_map<int, std::list<int>> adjacencyList;

public:
    /**
     * @brief Adiciona um vértice com a lista de adjacências.
     * @param u O vértice.
     * @param adjList A lista de adjacências.
     */
    void addNode(int u, const std::list<int> &adjList);

    /**
     * @brief Adiciona uma lista de adjacência ao grafo.
     * @param u O vértice ao qual a lista de adjacência será associada.
     * @param adjList A lista de vértices adjacentes ao vértice u.
     */
    void addAdjacencyList(int u, const std::list<int> &adjList);

    /**
     * @brief Remove o nó u do grafo.
     * @param u O nó a ser removido.
     * @return Nó removido
     */
    GraphNode popNode(int u);

    /**
     * @brief Obtém a lista de adjacência de um vértice.
     * @param u O vértice cuja lista de adjacência será obtida.
     * @return Uma referência à lista de vértices adjacentes.
     */
    const std::list<int> &getAdjacencyList(int u) const;

    /**
     * @brief Obtém o grau do vértice u.
     * @param u O vértice cujo grau será obtido.
     * @return O grau do vértice u.
     */
    int getDegree(int u) const;

    /**
     * @brief Verifica se o vértice u existe no grafo.
     * @param u O vértice a ser verificado.
     * @return True se o vértice u existe no grafo, false caso contrário.
     */
    bool hasNode(int u) const;

    /**
     * @brief Obtém todos os vértices do grafo.
     * @return Uma lista de vértices do grafo.
     */
    std::vector<int> getAllNodes() const;
};

#endif