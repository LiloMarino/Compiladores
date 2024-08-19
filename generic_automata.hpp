#ifndef GENERIC_AUTOMATA_HPP
#define GENERIC_AUTOMATA_HPP

#include "state.hpp"
#include <string>
#include <list>
#include <stack>
#include <set>
#include <tuple>

enum ActionType
{
    RAW_STRING,          // String Literal
    OPTION_INTERVAL_SET, // [-]?
    KLEENE_INTERVAL_SET, // [-]*
    PLUS_INTERVAL_SET,   // [-]+
    INTERVAL_SET,        // [-]
    OPTION_SET,          // []?
    KLEENE_SET,          // []*
    PLUS_SET,            // []+
    SET,                 // []
};

struct Action
{
    ActionType tipo;
    std::string str;
};

/**
 * @brief Classe de Autômato Genérico
 * @author Murilo Marino
 */
class GenericAutomata
{
private:
    int total_estados;
    State inicial;

public:
    /**
     * @brief Construtor do autômato genérico
     */
    GenericAutomata();

    /**
     * @brief Destrutor do autômato genérico
     */
    ~GenericAutomata();

    /**
     * @brief Implementa uma expressão regular no autômato
     * @param re Expressão regular
     */
    void addRegularExpression(const std::string &re);

    /**
     * @brief Obtém uma lista ordenada dos estados do autômato
     * @return Lista do Autômato
     */
    std::list<State *> toList();

    /**
     * @brief Iterador interno para usar em for-each,
     * @note Percorre o autômato utilizando o Depth-First Search (DFS)
     */
    class Iterator
    {
    private:
        std::stack<State *> stack;
        std::set<State *> visited;

    public:
        Iterator(State *root);
        bool operator!=(const Iterator &other) const;
        State &operator*();
        Iterator &operator++();
    };

    /**
     * @brief Obtém um iterador para o primeiro item
     * @return Retorna o iterador
     */
    Iterator begin();

    /**
     * @brief Obtém um iterador para o último item
     * @return Retorna o iterador
     */
    Iterator end();

private:
    /**
     * @brief Decodifica a expressão regular em ações para a montagem do autômato
     * @param re Expressão regular
     * @return Retorna uma lista de ações
     */
    std::list<Action> decodifyRegularExpression(const std::string &re);

    /**
     * @brief Obtém os intervalos
     * @param interval String contendo os intervalos
     * @return Retorna uma lista de tuplas de intervalos
     */
    std::list<std::tuple<int, int>> getIntervals(const std::string &interval);
};

#endif
