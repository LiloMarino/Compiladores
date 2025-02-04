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
            // Não há registrador livre: usa o algoritmo de spill
            // A função spill() (abaixo) fará:
            // - Selecionar entre os ativos o candidato com maior linha de última utilização,
            //   desempate por intervalo (menor duração) e, se necessário, por alocação mais recente.
            // - Se o candidato ativo tiver fim (última utilização) MAIOR que o fim do intervalo atual,
            //   ele é spillado (e seu registrador é liberado para o atual);
            // - Caso contrário, o próprio intervalo atual é spillado.
            bool allocated = spill(vr, iteration, registerMap, allocationOrder,
                                   availableRegisters, activeRegisters, spillIterations);
            // Se allocated == false, o vr corrente foi spillado (ou seja, registerMap[vr] == -1).
            // Caso contrário, já foi alocado usando o registrador do candidato spillado.
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

// ----------------------------------------------------------------------
// Método spill: realiza o algoritmo de spill para quando não há registrador livre.
// Parâmetros:
//   currentVr        - O registrador virtual do intervalo corrente.
//   iteration        - A iteração atual (usada para registrar qual intervalo foi spillado).
//   registerMap      - Mapeamento (por referência) do número de registrador atribuído a cada vr.
//   allocationOrder  - Mapeamento (por referência) da ordem em que cada vr foi alocado.
//   availableRegisters - Vetor (por referência) indicando quais registradores físicos estão livres.
//   activeRegisters  - Conjunto (por referência) dos vr atualmente ativos.
//   spillIterations  - Vetor (por referência) onde são registradas as iterações em que ocorreu spill.
bool LinearScan::spill(int currentVr, int iteration,
                       std::map<int, int> &registerMap,
                       std::unordered_map<int, int> &allocationOrder,
                       std::vector<bool> &availableRegisters,
                       std::set<int> &activeRegisters,
                       std::vector<int> &spillIterations)
{
    // Inicializa o candidato como o próprio nó atual.
    int candidate = currentVr;
    int candidateEnd = lifeTimes[currentVr].second;
    int candidateSpan = lifeTimes[currentVr].second - lifeTimes[currentVr].first;
    int candidateAllocOrder = iteration; // Para o nó atual, a ordem é a iteração corrente

    // Itera sobre os nós ativos para ver se algum deles deve ser spillado em vez do atual.
    for (int act : activeRegisters)
    {
        int actEnd = lifeTimes[act].second;
        int actSpan = lifeTimes[act].second - lifeTimes[act].first;
        int actAllocOrder = allocationOrder[act];

        // Critério 1: maior linha de última utilização (end)
        if (actEnd > candidateEnd)
        {
            candidate = act;
            candidateEnd = actEnd;
            candidateSpan = actSpan;
            candidateAllocOrder = actAllocOrder;
        }
        else if (actEnd == candidateEnd)
        {
            // Critério 2: se empatarem em end, o de menor intervalo de vida (end - start)
            if (actSpan < candidateSpan)
            {
                candidate = act;
                candidateEnd = actEnd;
                candidateSpan = actSpan;
                candidateAllocOrder = actAllocOrder;
            }
            else if (actSpan == candidateSpan)
            {
                // Critério 3: se ainda empatarem, o que foi alocado mais recentemente (maior allocationOrder)
                if (actAllocOrder > candidateAllocOrder)
                {
                    candidate = act;
                    candidateEnd = actEnd;
                    candidateSpan = actSpan;
                    candidateAllocOrder = actAllocOrder;
                }
            }
        }
    }

    // Se o candidato encontrado não for o nó atual, spilla o candidato ativo.
    if (candidate != currentVr)
    {
        int freedReg = registerMap[candidate];
        // Marca o candidato como spillado.
        registerMap[candidate] = -1;
        // Registra a iteração em que o spill ocorreu para o candidato.
        spillIterations.push_back(allocationOrder[candidate]);

        // Aloca o registrador liberado para o nó atual.
        registerMap[currentVr] = freedReg;
        allocationOrder[currentVr] = iteration;

        // Atualiza os nós ativos: remove o candidato spillado e insere o nó atual.
        activeRegisters.erase(candidate);
        activeRegisters.insert(currentVr);

        // O registrador liberado permanece marcado como ocupado.
        availableRegisters[freedReg] = false;
        return true; // currentVr foi alocado com sucesso
    }
    else
    {
        // Caso contrário, spilla o próprio nó atual.
        registerMap[currentVr] = -1;
        spillIterations.push_back(iteration); // registra a iteração corrente
        return false;                         // currentVr não foi alocado
    }
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
