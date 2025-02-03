#include "linearscan.hpp"
#include <iostream>
#include <vector>
#include <set>
#include <algorithm>

void LinearScan::start(const int K)
{
    this->K = K;
    std::vector<std::string> results;

    for (int currentK = K; currentK >= 2; --currentK)
    {
        std::cout << "K = " << currentK << std::endl << std::endl;
        bool success = allocateRegisters(currentK);
        results.push_back("K = " + std::to_string(currentK) + ": " + (success ? "Successful Allocation" : "SPILL"));
        std::cout << "----------------------------------------" << std::endl;
    }
    printSummary(results);
}

bool LinearScan::allocateRegisters(int currentK)
{
    std::unordered_map<int, int> registerMap;
    std::vector<bool> availableRegisters(currentK, true); // Aloca currentK registradores disponíveis
    std::set<int> activeRegisters;
    bool success = true;

    for (const auto &[vr, interval] : lifeTimes)
    {
        // Elimina os registradores expirados
        for (auto it = activeRegisters.begin(); it != activeRegisters.end();)
        {
            auto vr = *it; 
            if (lifeTimes[vr].second <= interval.first)
            {
                availableRegisters[registerMap[vr]] = true;
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
            // Encontrou um registrador disponível
            registerMap[vr] = reg;
            availableRegisters[reg] = false;
            activeRegisters.insert(vr);
        }
        else
        {
            // Não encontrou um registrador disponível, então faz spill
            std::cout << "Spill occurred for vr" << vr << std::endl;
            success = false;
        }
        std::cout << "vr" << vr << ": " << (reg != -1 ? std::to_string(reg) : "SPILL") << std::endl;
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
