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
    std::unordered_map<std::string, int> nao_terminais;            // Não terminal -> ID
    std::unordered_map<int, std::vector<std::string>> productions; // ID -> Produções
    std::unordered_map<int, GramaticaGroup> groups;                // ID -> (nullable,first,follow)
    int last_id;

public:
    /**
     * @brief Construtor da gramática
     */
    Gramatica();

    /**
     * @brief Adiciona uma produção a gramática
     * @param nao_terminal Não terminal
     * @param production Produção
     */
    void addProduction(const std::string &nao_terminal, const std::string &production);

    /**
     * @brief Preenche a tabela LL(1) do analisador sintático com base na gramática
     * @param sintatico Analisador Sintático
     */
    void toParsingTable(AnalisadorSintatico &sintatico);


    /**
     * @brief Verifica se um símbolo é um não terminal
     * @param nao_terminal Não terminal a ser verificado
     * @return Retorna verdadeiro se for não terminal e falso caso contrário
     */
    bool isNonTerminal(const std::string &nao_terminal);

    /**
     * @brief Printa a tabela de nullable, first, follow
     */
    void printGramaticaGroups() const;

private:
    /**
     * @brief Obtém o nullable, o first e o follow das produções
     */
    void getGramaticaGroups();
};

#endif