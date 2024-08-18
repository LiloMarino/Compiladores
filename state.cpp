#include "state.hpp"

State::State(int estado) : estado(estado)
{
}

void State::addTransition(char entrada, State *estado_destino)
{
    transitions.push_back({entrada, estado_destino});
}

int State::getEstado()
{
    return this->estado;
}

std::vector<Transition> State::getTransitions()
{
    return this->transitions;
}
