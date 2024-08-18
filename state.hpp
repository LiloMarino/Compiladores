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
     * @brief Getter de Estado
     * @return Retorna o número do estado
     */
    int getEstado();

    /**
     * @brief Getter das Transições
     * @return Retorna o vector de transições
     */
    std::vector<Transition> getTransitions();
};


#endif
