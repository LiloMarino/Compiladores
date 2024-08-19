#include "state.hpp"

State::State(int estado) : estado(estado)
{
}

void State::addTransition(char entrada, State *estado_destino)
{
    transitions.push_back({entrada, estado_destino});
}

void State::removeLambdaTransitions()
{
    for (auto it = transitions.begin(); it != transitions.end();)
    {  
        // Procura por uma transição lambda
        if (it->entrada == '\0')
        {
            it = transitions.erase(it); // Apaga a transição e atualiza o iterador
        }
        else
        {
            ++it; // Apenas avança para o próximo elemento se não apagar
        }
    }
}


int State::getEstado()
{
    return this->estado;
}

std::vector<Transition> State::getTransitions()
{
    return this->transitions;
}
