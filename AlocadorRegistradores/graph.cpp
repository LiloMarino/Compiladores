#include "graph.hpp"
#include <iostream>

void Graph::addNode(int u, const std::list<int> &adjList)
{
    adjacencyList[u] = adjList;
    for (int v : adjList)
    {
        adjacencyList[v].push_back(u);
    }
}

void Graph::addAdjacencyList(int u, const std::list<int> &adjList)
{
    adjacencyList[u].insert(adjacencyList[u].end(), adjList.begin(), adjList.end());
}

GraphNode Graph::popNode(int u)
{
    GraphNode node;
    node.virtualRegister = u;
    node.adjacencyList = adjacencyList[u];
    for (auto &neighbor : adjacencyList[u])
    {
        adjacencyList[neighbor].remove(u);
    }
    adjacencyList.erase(u);
    return node;
}

const std::list<int> &Graph::getAdjacencyList(int u) const
{
    return adjacencyList.at(u);
}

int Graph::getDegree(int u) const
{
    return adjacencyList.at(u).size();
}

bool Graph::hasNode(int u) const
{
    return adjacencyList.find(u) != adjacencyList.end();
}

std::vector<int> Graph::getAllNodes() const
{
    std::vector<int> nodes;
    for (const auto &pair : adjacencyList)
    {
        nodes.push_back(pair.first);
    }
    return nodes;
}