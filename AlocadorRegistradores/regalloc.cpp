#include "regalloc.hpp"
#include <stack>
#include <unordered_set>

RegAlloc::RegAlloc(std::unique_ptr<Graph> graph, int K)
    : graph(std::move(graph)), K(K) {}

void RegAlloc::start()
{
    std::stack<int> nodeStack;
    std::unordered_set<int> nodesToRemove;

    // Simplify phase: Trabalha com a cópia do grafo
    while (true)
    {
        bool removedNode = false;
        for (int node : graphCopy->getAllNodes())
        {
            if (graphCopy->getDegree(node) < K && nodesToRemove.find(node) == nodesToRemove.end())
            {
                nodesToRemove.insert(node);
                nodeStack.push(node);
                removedNode = true;
                break;
            }
        }
        if (!removedNode)
        {
            break;
        }
    }

    // Color phase
    while (!nodeStack.empty())
    {
        int node = nodeStack.top();
        nodeStack.pop();

        std::vector<bool> availableColors(K, true);

        for (int neighbor : graph->getAdjacencyList(node))
        {
            if (colorMap.find(neighbor) != colorMap.end())
            {
                availableColors[colorMap[neighbor]] = false;
            }
        }

        for (int color = 0; color < K; ++color)
        {
            if (availableColors[color])
            {
                colorMap[node] = color;
                break;
            }
        }

        graph->addEdge(node, node);
    }
}
