#include "linearscan.hpp"
#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <algorithm>

void LinearScan::start(const int K)
{
    this->K = K;
    std::vector<std::string> results;

    for (int currentK = K; currentK >= 2; --currentK)
    {
        std::cout << "K = " << currentK << std::endl;
        bool success = allocateRegisters(currentK);
        results.push_back(formatResult(currentK, success));
        std::cout << "----------------------------------------" << std::endl;
    }
    printSummary(results);
}

bool LinearScan::allocateRegisters(int currentK)
{
    std::map<int, int> registerMap;
    std::set<int> activeRegisters;
    bool success = true;

    for (const auto &[vr, interval] : lifeTimes)
    {
        removeExpiredIntervals(activeRegisters, interval.first);

        int reg = findAvailableRegister(activeRegisters, currentK);
        if (reg != -1)
        {
            registerMap[vr] = reg;
            activeRegisters.insert(reg);
        }
        else
        {
            handleSpill(vr);
            success = false;
        }
        std::cout << "vr" << vr << ": " << (reg != -1 ? std::to_string(reg) : "SPILL") << std::endl;
    }
    return success;
}

void LinearScan::removeExpiredIntervals(std::set<int> &activeRegisters, int currentLine)
{
    for (auto it = activeRegisters.begin(); it != activeRegisters.end();)
    {
        auto vr = *it;
        if (lifeTimes[vr].second < currentLine)
        {
            it = activeRegisters.erase(it);
        }
        else
        {
            ++it;
        }
    }
}

int LinearScan::findAvailableRegister(const std::set<int> &activeRegisters, int currentK)
{
    for (int reg = 0; reg < currentK; ++reg)
    {
        if (activeRegisters.find(reg) == activeRegisters.end())
        {
            return reg;
        }
    }
    return -1;
}

void LinearScan::handleSpill(int vr)
{
    std::cout << "Spill occurred for vr" << vr << std::endl;
}

std::string LinearScan::formatResult(int currentK, bool success)
{
    return "K = " + std::to_string(currentK) + ": " + (success ? "Successful Allocation" : "SPILL");
}

void LinearScan::printSummary(const std::vector<std::string> &results)
{
    std::cout << "----------------------------------------" << std::endl;
    for (const auto &result : results)
    {
        std::cout << result << std::endl;
    }
}

void LinearScan::addVirtualRegister(int virtualRegister, std::list<int> lifeTime)
{
    int start = lifeTime.front();
    int end = lifeTime.back();
    lifeTimes[virtualRegister] = {start, end};
}
