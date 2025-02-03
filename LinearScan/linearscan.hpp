#ifndef LINEARSCAN_HPP
#define LINEARSCAN_HPP

#include <list>
#include <unordered_map>

class LinearScan
{
private:
    int K;
    std::unordered_map<int, std::pair<int,int>> lifeTimes;
public:
    /**
     * @brief Inicia o algoritmo do linear scan
     * @param K Número de registradores
     */
    void start(const int K);

    /**
     * @brief Adiciona um registrador virtual
     * @param virtualRegister Número do registrador virtual
     * @param lifeTime Intervalo de tempo de vida
     */
    void addVirtualRegister(int virtualRegister, std::list<int> lifeTime);
};

#endif