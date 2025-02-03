#include "linearscan.hpp"
#include <iostream>
#include <vector>
#include <set>
#include <algorithm>
#include <sstream>

void LinearScan::start(const int K)
{
    this->K = K;
    std::vector<std::string> results;

    for (int currentK = K; currentK >= 2; --currentK)
    {
        std::cout << "K = " << currentK << std::endl << std::endl;
        
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

bool LinearScan::allocateRegisters(int currentK, std::vector<int>& spillIterations)
{
    std::unordered_map<int, int> registerMap;
    std::vector<bool> availableRegisters(currentK, true); // currentK registradores disponíveis
    std::set<int> activeRegisters;
    bool success = true;
    
    int iteration = 1;
    for (const auto &[vr, interval] : lifeTimes)
    {
        // Remove registradores expirados
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

        // Procura um registrador disponível
        int reg = -1;
        for (int i = 0; i < currentK; ++i)
        {
            if (availableRegisters[i])
            {
                reg = i;
                break;
            }
        }

        if (reg != -1)
        {
            // Aloca o registrador encontrado
            registerMap[vr] = reg;
            availableRegisters[reg] = false;
            activeRegisters.insert(vr);
        }
        else
        {
            // Se não houver registrador disponível, registra a iteração do spill
            spillIterations.push_back(iteration);
            success = false;
        }

        // Imprime a alocação para este registrador
        std::cout << "vr" << vr << ": " << (reg != -1 ? std::to_string(reg) : "SPILL") << std::endl;
        iteration++;
    }
    return success;
}

void LinearScan::printSummary(const std::vector<std::string> &results)
{
    std::cout << "----------------------------------------";
    for (const auto &result : results)
    {
        std::cout << std::endl << result;
    }
}

void LinearScan::addVirtualRegister(int virtualRegister, std::list<int> lifeTime)
{
    int start = lifeTime.front();
    int end = lifeTime.back();
    lifeTimes[virtualRegister] = {start, end};
}
