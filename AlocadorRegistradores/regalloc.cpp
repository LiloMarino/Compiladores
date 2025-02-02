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

bool  RegAlloc::isVirtual(int node)
{
    return node >= K;
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
                      // Ordena de forma crescente pelo menor grau e, em caso de empate, pelo menor número
                      return (degreeA < degreeB) || (degreeA == degreeB && a < b);
                  });

        
        // Procura e remove o registrador virtual (node >= K) que possui grau menor que currentK
        int nodeToRemove = -1;
        for (auto &node : nodes)
        {
            if (isVirtual(node) && graph->getDegree(node) < currentK)
            {
                nodeToRemove = node;
                break;
            }
        }

        if (nodeToRemove == -1)
        {
            // Não encontrou um registrador virtual com grau menor que currentK
            
            // Cria um vetor com apenas os registradores virtuais
            std::vector<int> virtualNodes;
            std::copy_if(nodes.begin(), nodes.end(), std::back_inserter(virtualNodes),
                        [this](int node) { return isVirtual(node); });

            if (!virtualNodes.empty())
            {
                // Existe nós virtuais, então procura e marca um nó para spill
                int spillNode = spill(virtualNodes);
                std::cout << "Push: " << spillNode << " *" << std::endl;
                nodeStack.push(graph->popNode(spillNode));
            }
            else
            {
                // Se não houver nenhum nó virtual (ou seja, só há nós físicos), podemos sair do laço.
                break;
            }
        }
        else
        {
            // Encontrou um registrador virtual com grau menor que currentK então remove
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
    std::unordered_map<int, int> colorMap; // Mapa de cores dos nós
    std::vector<bool> availableColors(currentK, true); // Aloca currentK cores disponíveis
    bool success = true;

    while (!nodeStack.empty())
    {
        auto node = nodeStack.top();
        nodeStack.pop();

        // Reseta todas as cores para disponíveis
        std::fill(availableColors.begin(), availableColors.end(), true);
        
        // Marca as cores usadas pelos vizinhos
        for (int neighbor : node.adjacencyList) 
        {
            if (neighbor < currentK)
            {
                // Vizinho é um registrador físico
                availableColors[neighbor] = false; 
            }
            else if (colorMap.find(neighbor) != colorMap.end())
            {
                // Vizinho é um registrador virtual com cor atribuída
                availableColors[colorMap[neighbor]] = false;
            }
            else {
                // Vizinho é um registrador virtual sem cor atribuída
                std::cerr << "Vizinho sem cor atribuída: " << neighbor << std::endl;
            }
        }

        // Tenta atribuir a cor ao nó
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
            // Encontrou uma cor disponível
            colorMap[node.virtualRegister] = assignedColor;
            std::cout << "Pop: " << node.virtualRegister << " -> " << assignedColor << std::endl;
        }
        else
        {
            // Não encontrou uma cor disponível
            std::cout << "Pop: " << node.virtualRegister << " -> NO COLOR AVAILABLE" << std::endl; 
            success = false;
            break;
        }

        graph->addNode(node.virtualRegister, node.adjacencyList);
    }
    return success;
}
