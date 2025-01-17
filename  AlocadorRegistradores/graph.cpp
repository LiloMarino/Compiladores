#include "graph.hpp"
#include <iostream>

void Graph::addEdge(int u, int v) {
    adjacencyList[u].push_back(v);
}

void Graph::addAdjacencyList(int u, const std::list<int>& adjList) {
    adjacencyList[u].insert(adjacencyList[u].end(), adjList.begin(), adjList.end());
}

void Graph::removeEdge(int u, int v) {
    adjacencyList[u].remove(v);
}

const std::list<int>& Graph::getAdjacencyList(int u) const {
    return adjacencyList.at(u);
}

void Graph::display() const {
    for (const auto& pair : adjacencyList) {
        std::cout << pair.first << ": ";
        for (int v : pair.second) {
            std::cout << v << " ";
        }
        std::cout << std::endl;
    }
}