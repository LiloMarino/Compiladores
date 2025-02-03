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
    // Mapeia cada vr para o número do registrador atribuído (ou -1 para spill)
    std::unordered_map<int, int> registerMap;
    std::vector<bool> availableRegisters(currentK, true);
    std::set<int> activeRegisters;
    // Armazena a ordem (a iteração em que foi alocado) para desempate no spill
    std::unordered_map<int, int> allocationOrder;

    // Para a impressão ao final, vamos armazenar os resultados (usando a ordem dos vr)
    std::vector<int> vrOrder;

    int iteration = 0;
    
    // Para processar os intervalos na ordem crescente do número do registrador virtual,
    // extraímos os números e os ordenamos.
    for (const auto &p : lifeTimes)
    {
        vrOrder.push_back(p.first);
    }
    std::sort(vrOrder.begin(), vrOrder.end());
    
    // Percorre cada virtual register (em ordem) e realiza a alocação ou spill
    for (int vr : vrOrder)
    {
        // Recupera o intervalo de tempo de vida deste vr: {start, end}
        auto interval = lifeTimes[vr];
        
        // Remove os registradores ativos cujos intervalos já expiraram (ou seja, cujo final é <= o início atual)
        std::vector<int> expired;
        for (int act : activeRegisters)
        {
            if ( lifeTimes[act].second <= interval.first )
            {
                availableRegisters[ registerMap[act] ] = true;
                expired.push_back(act);
            }
        }
        for (int act : expired)
        {
            activeRegisters.erase(act);
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
    for (int vr : vrOrder)
    {
        std::cout << "vr" << vr << ": " 
                  << ( registerMap[vr] == -1 ? "SPILL" : std::to_string(registerMap[vr]) )
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
                       std::unordered_map<int, int>& registerMap,
                       std::unordered_map<int, int>& allocationOrder,
                       std::vector<bool>& availableRegisters,
                       std::set<int>& activeRegisters,
                       std::vector<int>& spillIterations)
{
    // Seleciona o candidato entre os intervalos ativos de acordo com os critérios.
    int candidate = -1;
    for (int act : activeRegisters)
    {
        if (candidate == -1)
            candidate = act;
        else
        {
            // Critério 1: maior linha de última utilização (lifeTimes[vr].second)
            if ( lifeTimes[act].second > lifeTimes[candidate].second )
            {
                candidate = act;
            }
            else if ( lifeTimes[act].second == lifeTimes[candidate].second )
            {
                // Critério 2: em caso de empate, o de menor intervalo de vida
                int lenAct = lifeTimes[act].second - lifeTimes[act].first;
                int lenCand = lifeTimes[candidate].second - lifeTimes[candidate].first;
                if (lenAct < lenCand)
                {
                    candidate = act;
                }
                else if (lenAct == lenCand)
                {
                    // Critério 3: se ainda houver empate, o que foi alocado mais recentemente (maior allocationOrder)
                    if ( allocationOrder[act] > allocationOrder[candidate] )
                        candidate = act;
                }
            }
        }
    }
    
    // Decide se deve spillar o candidato ativo ou o próprio intervalo atual.
    // Se o candidato ativo tiver fim MAIOR que o fim do intervalo atual, então
    // ele é spillado e seu registrador é atribuído ao atual.
    if ( candidate != -1 && lifeTimes[candidate].second > lifeTimes[currentVr].second )
    {
        // Libera o registrador ocupado pelo candidato
        int freedReg = registerMap[candidate];
        registerMap[candidate] = -1;  // marca o candidato como spillado
        spillIterations.push_back( allocationOrder[candidate] ); // registra a iteração em que o candidato foi spillado
        
        // Aloca o registrador liberado para o intervalo atual
        registerMap[currentVr] = freedReg;
        allocationOrder[currentVr] = iteration;
        // Atualiza o estado: o candidato sai dos ativos e o atual passa a ser ativo
        activeRegisters.erase(candidate);
        activeRegisters.insert(currentVr);
        // O registrador liberado permanece marcado como ocupado (não disponível)
        availableRegisters[freedReg] = false;
        return true; // currentVr foi alocado
    }
    else
    {
        // Caso contrário, o próprio intervalo atual é spillado.
        registerMap[currentVr] = -1; // marca como spill
        spillIterations.push_back(iteration);  // registra a iteração corrente
        return false; // currentVr não foi alocado
    }
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
