#include "regalloc.hpp"
#include <stack>
#include <iostream>
#include <algorithm>
#include <map>

RegAlloc::RegAlloc(std::unique_ptr<Graph> graph, int K)
    : graph(std::move(graph)), K(K) {}

void RegAlloc::start()
{
    // Simplificação
    std::stack<GraphNode> nodeStack;
    for (int currentK = K; currentK >= 2; --currentK)
    {
        simplify(nodeStack, currentK);
    }
}

void RegAlloc::simplify(std::stack<GraphNode> &nodeStack, int currentK)
{
    while (!graph->getAllNodes().empty())
    {
        auto nodes = graph->getAllNodes();
        std::sort(nodes.begin(), nodes.end(),
                  [this](int a, int b)
                  {
                      int degreeA = graph->getDegree(a);
                      int degreeB = graph->getDegree(b);
                      // Em caso de nós com o mesmo grau, o nó com o menor número de registrador virtual será escolhido.
                      return (degreeA < degreeB) || (degreeA == degreeB && a < b);
                  });
        int nodeToRemove = -1;
        for (auto &node : nodes)
        {
            if (graph->getDegree(node) < currentK)
            {
                nodeToRemove = node;
                break;
            }
        }
        if (nodeToRemove == -1)
        {
            // Alguma lógica
            break;
        }
        nodeStack.push(graph->popNode(nodeToRemove));
        std::cout << "Push: " << nodeToRemove << std::endl;
    }
}

void RegAlloc::spill(std::stack<GraphNode> &nodeStack)
{
}

void RegAlloc::select(std::stack<GraphNode> &nodeStack)
{
}
