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
     * @return true se a alocação foi bem-sucedida, false se ocorreu spill
     */
    bool allocateRegisters(int currentK);

    /**
     * @brief Remove os intervalos expirados da lista de registradores ativos
     * @param activeRegisters Conjunto de registradores ativos
     * @param currentLine Linha atual do tempo de vida
     */
    void removeExpiredIntervals(std::set<int> &activeRegisters, int currentLine);

    /**
     * @brief Encontra um registrador disponível
     * @param activeRegisters Conjunto de registradores ativos
     * @param currentK Número atual de registradores físicos
     * @return Número do registrador disponível ou -1 se nenhum estiver disponível
     */
    int findAvailableRegister(const std::set<int> &activeRegisters, int currentK);

    /**
     * @brief Trata a ocorrência de spill
     * @param vr Registrador virtual que sofreu spill
     */
    void handleSpill(int vr);

    /**
     * @brief Formata o resultado da alocação
     * @param currentK Número atual de registradores físicos
     * @param success Indica se a alocação foi bem-sucedida
     * @return String formatada com o resultado da alocação
     */
    std::string formatResult(int currentK, bool success);

    /**
     * @brief Imprime o resumo dos resultados da alocação
     * @param results Vetor contendo os resultados formatados
     */
    void printSummary(const std::vector<std::string> &results);
};

#endif