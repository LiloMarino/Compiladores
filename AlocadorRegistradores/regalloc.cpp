#include "regalloc.hpp"
#include <stack>
#include <iostream>
#include <algorithm>
#include <map>

RegAlloc::RegAlloc(std::unique_ptr<Graph> graph, int K)
    : graph(std::move(graph)), K(K) {}

void RegAlloc::start()
{
    std::stack<GraphNode> nodeStack;
    for (int currentK = K; currentK >= 2; --currentK)
    {
        printLine();
        std::cout << "K = " << currentK << std::endl
                  << std::endl;
        simplify(nodeStack, currentK);
        select(nodeStack, currentK);
    }
}

int RegAlloc::getK()
{
    return K;
}

void RegAlloc::printLine()
{
    std::cout << "----------------------------------------" << std::endl;
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
            if (node >= K && graph->getDegree(node) < currentK)
            {
                nodeToRemove = node;
                break;
            }
        }
        if (nodeToRemove == -1)
        {
            // Não houve um nó cujo o grau seja menor que K
            spill(nodes);
            break;
        }
        nodeStack.push(graph->popNode(nodeToRemove));
        std::cout << "Push: " << nodeToRemove << std::endl;
    }
}

int RegAlloc::spill(std::vector<int> &nodes)
{
    auto maxDegreeNode = std::max_element(nodes.begin(), nodes.end(),
                                          [this](int a, int b)
                                          {
                                              // Obtém o nó de maior grau, em caso de empate obtém o de menor número
                                              return graph->getDegree(a) < graph->getDegree(b) ||
                                                     (graph->getDegree(a) == graph->getDegree(b) && a < b);
                                          });
    std::cout << "Potential Spill: " << *maxDegreeNode << std::endl;
    return *maxDegreeNode;
}

void RegAlloc::select(std::stack<GraphNode> &nodeStack, int currentK)
{
    std::unordered_map<int, int> colorMap;

    while (!nodeStack.empty())
    {
        auto node = nodeStack.top();
        nodeStack.pop();

        // Todas as cores estão disponíveis inicialmente
        std::vector<bool> availableColors(currentK, true);

        // Marca as cores dos vizinhos como indisponíveis
        for (int neighbor : node.adjacencyList)
        {
            if (neighbor < K)
            {
                availableColors[neighbor] = false;
            }
            else if (colorMap.find(neighbor) != colorMap.end())
            {
                int usedColor = colorMap[neighbor];
                if (usedColor < currentK)
                {
                    availableColors[usedColor] = false;
                }
            }
        }

        // Atribui a primeira cor disponível ao nó atual
        int assignedColor = -1;
        for (int color = 0; color < currentK; ++color)
        {
            if (availableColors[color])
            {
                assignedColor = color;
                break;
            }
        }

        if (assignedColor != -1)
        {
            colorMap[node.virtualRegister] = assignedColor;
            std::cout << "Pop: " << node.virtualRegister << " -> " << assignedColor << std::endl;
        }
        else
        {
            std::cout << "Pop: " << node.virtualRegister << " -> NO COLOR AVAILABLE" << std::endl;
        }

        graph->addNode(node.virtualRegister, node.adjacencyList);
    }
}
