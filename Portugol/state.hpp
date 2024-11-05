#ifndef STATE_HPP
#define STATE_HPP

#include <vector>

// Define antecipademente para usar na struct
class State;

struct Transition
{
    char entrada;
    State *estado_destino;
};

/**
 * @brief Classe de Estados
 * @author Murilo Marino
 */
class State
{
private:
    int estado;
    std::vector<Transition> transitions;
public:
    /**
     * @brief Construtor de um estado
     * @param estado Número do estado
     */
    State(int estado);

    /**
     * @brief Adiciona transições ao estado
     * @param entrada Caractere de entrada
     * @param estado_destino Estado de destino
     */
    void addTransition(char entrada, State* estado_destino);

    /**
     * @brief Remove todas as transições lambda do estado
     */
    void removeLambdaTransitions();

    /**
     * @brief Getter de Estado
     * @return Retorna o número do estado
     */
    int getEstado() const;

    /**
     * @brief Getter das Transições
     * @return Retorna o vector de transições
     */
    std::vector<Transition> getTransitions() const;

    /**
     * @brief Setter das Transições
     */
    void setTransitions(const std::vector<Transition>& newTransitions);
};


#endif
