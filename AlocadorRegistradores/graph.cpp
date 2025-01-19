#include "graph.hpp"
#include <iostream>

void Graph::addEdge(int u, int v)
{
    adjacencyList[u].push_back(v);
}

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

void Graph::removeEdge(int u, int v)
{
    adjacencyList[u].remove(v);
}

void Graph::removeNode(int u)
{
    for (auto &neighbor : adjacencyList[u])
    {
        adjacencyList[neighbor].remove(u);
    }
    adjacencyList.erase(u);
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

void Graph::display() const
{
    for (const auto &pair : adjacencyList)
    {
        std::cout << pair.first << ": ";
        for (int v : pair.second)
        {
            std::cout << v << " ";
        }
        std::cout << std::endl;
    }
}