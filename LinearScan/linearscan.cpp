#include "linearscan.hpp"
#include <iostream>
#include <vector>
#include <set>
#include <algorithm>
#include <sstream>
#include <unordered_map>

void LinearScan::start(const int K)
{
    this->K = K;
    std::vector<std::string> results;

    for (int currentK = K; currentK >= 2; --currentK)
    {
        std::cout << "K = " << currentK << std::endl
                  << std::endl;

        // Vetor para armazenar as iterações onde ocorreu spill
        std::vector<int> spillIterations;
        bool success = allocateRegisters(currentK, spillIterations);

        // Se houver spill, adiciona a informação das iterações na mensagem do resumo
        if (success)
        {
            results.push_back("K = " + std::to_string(currentK) + ": Successful Allocation");
        }
        else
        {
            std::ostringstream oss;
            oss << "K = " << currentK << ": SPILL on interation(s): ";
            for (size_t i = 0; i < spillIterations.size(); ++i)
            {
                if (i > 0)
                    oss << ", ";
                oss << spillIterations[i];
            }
            results.push_back(oss.str());
        }
        std::cout << "----------------------------------------" << std::endl;
    }
    printSummary(results);
}

bool LinearScan::allocateRegisters(int currentK, std::vector<int> &spillIterations)
{
    // Mapeia cada vr para o número do registrador atribuído (ou -1 para spill)
    std::map<int, int> registerMap;
    std::vector<bool> availableRegisters(currentK, true);
    std::set<int> activeRegisters;
    std::unordered_map<int, int> allocationOrder;

    int iteration = 0;
    for (const auto &[vr, interval] : lifeTimes)
    {
        // Remove os registradores ativos cujos intervalos já expiraram (ou seja, cujo final é <= o início atual)
        for (auto it = activeRegisters.begin(); it != activeRegisters.end();)
        {
            int activeVr = *it;
            if (lifeTimes[activeVr].second <= interval.first)
            {
                availableRegisters[registerMap[activeVr]] = true;
                it = activeRegisters.erase(it);
            }
            else
            {
                ++it;
            }
        }

        // Procura um registrador físico livre
        int freeReg = -1;
        for (int i = 0; i < currentK; ++i)
        {
            if (availableRegisters[i])
            {
                freeReg = i;
                break;
            }
        }

        if (freeReg != -1)
        {
            // Se há registrador disponível, aloca normalmente
            registerMap[vr] = freeReg;
            allocationOrder[vr] = iteration;
            availableRegisters[freeReg] = false;
            activeRegisters.insert(vr);
        }
        else
        {
            // Não há registrador livre usa o algoritmo de spill
            spillIterations.push_back(iteration);
            
            // Inicializa o candidato como o próprio nó atual.
            int candidate = vr;
            int candidateEnd = lifeTimes[vr].second;
            int candidateLifeTime = lifeTimes[vr].second - lifeTimes[vr].first;
            int candidateAllocOrder = iteration;

            // Itera sobre os nós ativos para ver se algum deles deve ser spillado em vez do atual.
            for (int act : activeRegisters)
            {
                int actEnd = lifeTimes[act].second;
                int actLifeTime = lifeTimes[act].second - lifeTimes[act].first;
                int actAllocOrder = allocationOrder[act];

                if (actEnd > candidateEnd)
                {
                    // Critério 1: maior linha de última utilização (end)
                    candidate = act;
                    candidateEnd = actEnd;
                    candidateLifeTime = actLifeTime;
                    candidateAllocOrder = actAllocOrder;
                }
                else if (actEnd == candidateEnd)
                {
                    // Critério 2: se empatarem em end, o de menor intervalo de vida
                    if (actLifeTime < candidateLifeTime)
                    {
                        candidate = act;
                        candidateEnd = actEnd;
                        candidateLifeTime = actLifeTime;
                        candidateAllocOrder = actAllocOrder;
                    }
                    else if (actLifeTime == candidateLifeTime)
                    {
                        // Critério 3: se ainda empatarem, o que foi alocado mais recentemente (maior allocationOrder)
                        if (actAllocOrder > candidateAllocOrder)
                        {
                            candidate = act;
                            candidateEnd = actEnd;
                            candidateLifeTime = actLifeTime;
                            candidateAllocOrder = actAllocOrder;
                        }
                    }
                }
            }

            if (candidate != vr)
            {
                // O candidato não é o nó atual
                int spilledReg = registerMap[candidate];
                registerMap[candidate] = -1;

                // Aloca o registrador liberado para o nó atual.
                registerMap[vr] = spilledReg;
                allocationOrder[vr] = iteration;

                // Atualiza os nós ativos: remove o candidato spillado e insere o nó atual.
                activeRegisters.erase(candidate);
                activeRegisters.insert(vr);

                // O registrador liberado permanece marcado como ocupado.
                availableRegisters[spilledReg] = false;
            }
            else
            {
                // O candidato é o nó atual, então spilla ele
                registerMap[vr] = -1;
            }
        }

        iteration++;
    }

    // Ao final, imprime a alocação de cada registrador virtual, na ordem (usando -1 como flag para SPILL)
    for (const auto &[vr, fr] : registerMap)
    {
        std::cout << "vr" << vr << ": "
                  << (fr == -1 ? "SPILL" : std::to_string(fr))
                  << std::endl;
    }

    // Retorna true se nenhuma iteração teve spill; caso contrário, false.
    return spillIterations.empty();
}

void LinearScan::printSummary(const std::vector<std::string> &results)
{
    std::cout << "----------------------------------------";
    for (const auto &result : results)
    {
        std::cout << std::endl
                  << result;
    }
}

void LinearScan::addVirtualRegister(int virtualRegister, std::list<int> lifeTime)
{
    int start = lifeTime.front();
    int end = lifeTime.back();
    lifeTimes[virtualRegister] = {start, end};
}
