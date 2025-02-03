#ifndef LINEARSCAN_HPP
#define LINEARSCAN_HPP

#include <list>
#include <unordered_map>
#include <vector>
#include <set>
#include <string>

class LinearScan
{
private:
    int K;
    std::unordered_map<int, std::pair<int, int>> lifeTimes; 

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

private:
    /**
     * @brief Aloca os registradores para os tempos de vida
     * @param currentK Número atual de registradores físicos
     * @param spillIterations Vetor que armazenará as iterações (ordem dos registradores) onde ocorreu spill
     * @return true se a alocação foi bem-sucedida, false se ocorreu spill
     */
    bool allocateRegisters(int currentK, std::vector<int>& spillIterations);

    /**
     * @brief Imprime o resumo dos resultados da alocação
     * @param results Vetor contendo os resultados formatados
     */
    void printSummary(const std::vector<std::string> &results);

    /**
     * @brief Realiza o spill
     */
    void spill();
};

#endif
