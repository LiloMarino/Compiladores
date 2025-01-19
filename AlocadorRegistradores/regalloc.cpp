#include "regalloc.hpp"
#include <stack>
#include <iostream>
#include <algorithm>
#include <map>

RegAlloc::RegAlloc(std::unique_ptr<Graph> graph, int K)
    : graph(std::move(graph)), K(K) {}

void RegAlloc::start()
{
    for (int currentK = K; currentK >= 2; --currentK)
    {
        std::stack<int> nodeStack;
        std::map<int, int> localColorMap;
        std::map<int, std::list<int>> originalAdjacency;
        bool spillOccurred = false;

        // Simplify phase
        while (!graph->getAllNodes().empty())
        {
            auto nodes = graph->getAllNodes();
            std::sort(nodes.begin(), nodes.end(), [this](int a, int b) {
                int degreeA = graph->getDegree(a);
                int degreeB = graph->getDegree(b);
                return (degreeA < degreeB) || (degreeA == degreeB && a < b);
            });

            int nodeToRemove = -1;
            for (int node : nodes)
            {
                if (graph->getDegree(node) < currentK)
                {
                    nodeToRemove = node;
                    break;
                }
            }

            if (nodeToRemove == -1)
            {
                auto maxDegreeNode = std::max_element(nodes.begin(), nodes.end(), [this](int a, int b) {
                    return graph->getDegree(a) < graph->getDegree(b) || 
                           (graph->getDegree(a) == graph->getDegree(b) && a < b);
                });
                nodeToRemove = *maxDegreeNode;
                spillOccurred = true;
                std::cout << "Potential Spill: " << nodeToRemove << std::endl;
            }

            std::cout << "Push: " << nodeToRemove << std::endl;
            originalAdjacency[nodeToRemove] = graph->getAdjacencyList(nodeToRemove);
            nodeStack.push(nodeToRemove);
            graph->removeNode(nodeToRemove);
        }

        // Color phase
        while (!nodeStack.empty())
        {
            int node = nodeStack.top();
            nodeStack.pop();

            std::vector<bool> availableColors(currentK, true);

            for (int neighbor : originalAdjacency[node])
            {
                if (localColorMap.find(neighbor) != localColorMap.end())
                {
                    availableColors[localColorMap[neighbor]] = false;
                }
            }

            bool colored = false;
            for (int color = 0; color < currentK; ++color)
            {
                if (availableColors[color])
                {
                    localColorMap[node] = color;
                    colored = true;
                    break;
                }
            }

            if (!colored)
            {
                std::cout << "Pop: " << node << " -> NO COLOR AVAILABLE" << std::endl;
                spillOccurred = true;
            }
            else
            {
                std::cout << "Pop: " << node << " -> " << localColorMap[node] << std::endl;
            }

            graph->addNode(node, originalAdjacency[node]);
        }

        std::cout << "Graph -> K = " << currentK << ": " << (spillOccurred ? "SPILL" : "Successful Allocation") << std::endl;
    }
}
