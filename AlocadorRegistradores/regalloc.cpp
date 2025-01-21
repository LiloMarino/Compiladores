#include "regalloc.hpp"
#include <stack>
#include <iostream>
#include <algorithm>
#include <map>
#include <iomanip>

RegAlloc::RegAlloc(std::unique_ptr<Graph> graph, int K)
    : graph(std::move(graph)), K(K) {}

void RegAlloc::start(int graphId)
{
    this->graphId = graphId;
    std::cout << "Graph " << graphId << " -> " << "Physical Registers: " << K << std::endl;
    printLine();
    std::vector<std::string> results;
    for (int currentK = K; currentK >= 2; --currentK)
    {
        std::stack<GraphNode> nodeStack;
        printLine();
        std::cout << "K = " << currentK << std::endl
                  << std::endl;

        simplify(nodeStack, currentK);
        bool success = select(nodeStack, currentK);

        // Use std::setw para garantir alinhamento
        std::ostringstream oss;
        oss << "Graph " << graphId << " -> K = " << std::setw(2) << currentK << ": "
            << (success ? "Successful Allocation" : "SPILL");
        results.push_back(oss.str());
    }
    printLine();
    printSummary(results);
}

void RegAlloc::printSummary(const std::vector<std::string> &results)
{
    printLine(false);
    for (const auto &result : results)
    {
        std::cout << std::endl << result;
    }
}

int RegAlloc::getK()
{
    return K;
}

void RegAlloc::printLine(bool endLine)
{
    std::cout << "----------------------------------------";
    if (endLine) {
        std::cout << std::endl;
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
            int spillNode = spill(nodes);
            std::cout << "Push: " << spillNode << " *" << std::endl;
            nodeStack.push(graph->popNode(spillNode));
            break;
        }
        else
        {
            std::cout << "Push: " << nodeToRemove << std::endl;
            nodeStack.push(graph->popNode(nodeToRemove));
        }
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
    return *maxDegreeNode;
}

bool RegAlloc::select(std::stack<GraphNode> &nodeStack, int currentK)
{
    std::unordered_map<int, int> colorMap;
    bool success = true;

    while (!nodeStack.empty())
    {
        auto node = nodeStack.top();
        nodeStack.pop();

        std::vector<bool> availableColors(currentK, true);

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
            success = false;
        }

        graph->addNode(node.virtualRegister, node.adjacencyList);
    }
    return success;
}
