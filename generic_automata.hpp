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
    std::list<State> estados;
public:
    /**
     * @brief Construtor do autômato genérico
     */
    GenericAutomata();

    /**
     * @brief Implementa uma expressão regular no autômato
     * @param re Expressão regular
     */
    void addRegularExpression(const std::string &re);

    /**
     * @brief Cria um novo estado com a próxima numeração e retorna
     * @return Retorna o estado
     */
    State* createNewState();

    /**
     * @brief Obtém uma lista ordenada dos estados do autômato
     * @return Lista do Autômato
     */
    std::list<State*> toList();

    /**
     * @brief Encontra um estado na lista de estados
     * @param estadoNum Número do estado a ser encontrado
     * @return Retorna um ponteiro para o estado correspondente ou nullptr se não encontrado
     */
    State* findState(int estadoNum) const;

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
