#ifndef GRAMATICA_HPP
#define GRAMATICA_HPP

#include "sintatico.hpp"
#include <unordered_map>
#include <map>
#include <unordered_set>
#include <string>
#include <vector>

struct GramaticaGroup
{
    bool nullable;
    std::unordered_set<std::string> first;
    std::unordered_set<std::string> follow;
};

/**
 * @class Classe de Gramática
 * @author Murilo Marino
 */
class Gramatica
{
private:
    std::unordered_map<std::string, int> nao_terminais;                        // Não terminal -> ID
    std::unordered_map<int, std::vector<std::deque<std::string>>> productions; // ID -> Produções
    std::unordered_map<int, GramaticaGroup> groups;                            // ID -> (nullable,first,follow)
    int last_id;

public:
    /**
     * @brief Construtor da gramática
     */
    Gramatica();

    /**
     * @brief Adiciona uma produção à gramática
     * @param nao_terminal Não terminal
     * @param produto Resultado da produção
     * @warning Os símbolos do produto devem ser separados por espaço
     */
    void addProduction(const std::string &nao_terminal, const std::string &produto);

    /**
     * @brief Preenche a tabela LL(1) do analisador sintático com base na gramática
     * @param sintatico Analisador Sintático
     */
    void toParsingTable(AnalisadorSintatico &sintatico);

    /**
     * @brief Identifica problemas para usar a gramática como LL(1)
     */
    void identifyConflicts();

    /**
     * @brief Verifica se um símbolo é um não terminal
     * @param nao_terminal Não terminal a ser verificado
     * @return Retorna verdadeiro se for não terminal e falso caso contrário
     */
    bool isNonTerminal(const std::string &nao_terminal) const;

    /**
     * @brief Printa a tabela de nullable, first, follow
     */
    void printGramaticaGroups() const;

    /**
     * @brief Printa a gramática
     */
    void printGramatica() const;

private:
    /**
     * @brief Obtém o conjunto FIRST de uma produção
     * @param production Produção a ser obtida o FIRST
     * @return Retorna o FIRST da produção
     * @warning O map groups deve estar completo
     */
    std::unordered_set<std::string> getFirst(const std::string &n_terminal, const std::deque<std::string> &production);

    /**
     * @brief Obtém a string de um não terminal pelo seu id
     * @param id Id do não terminal
     * @return Retorna a string do não terminal
     */
    std::string getNaoTerminalById(int id) const;

    /**
     * @brief Obtém o nullable, o first e o follow das produções
     */
    void getGramaticaGroups();
};

#endif