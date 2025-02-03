#include "linearscan.hpp"
#include <iostream>

void LinearScan::start(const int K)
{
    this->K = K;
}

void LinearScan::addVirtualRegister(int virtualRegister, std::list<int> lifeTime)
{
    int start = lifeTime.front();
    int end = lifeTime.back();
    lifeTimes[virtualRegister] = {start, end};
}
