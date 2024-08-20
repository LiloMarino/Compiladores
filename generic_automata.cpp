#include "generic_automata.hpp"
#include <iostream>

GenericAutomata::GenericAutomata(): inicial(nullptr)
{
}

void GenericAutomata::addRegularExpression(const std::string &re)
{
    std::list<Action> actions = this->decodifyRegularExpression(re);
    std::list<State *> states_list;
    State *first_state = this->createNewState();
    for (auto action : actions)
    {
        State *state; // Variável auxiliar
        switch (action.tipo)
        {
        case RAW_STRING:
            for (char c : action.str)
            {
                state = this->createNewState();
                if (states_list.empty())
                {
                    // É a primeira transição
                    first_state->addTransition(c, state);
                }
                else
                {
                    // É uma transição posterior
                    State *last_state = states_list.back();
                    last_state->addTransition(c, state);
                }
                states_list.push_back(state);
            }
            break;
        case OPTION_INTERVAL_SET:
            state = this->createNewState();
            if (states_list.empty())
            {
                // É a primeira transição
                first_state->addTransition('\0', state);
                for (const auto &interval : this->getIntervals(action.str))
                {
                    for (int i = std::get<0>(interval); i < std::get<1>(interval); ++i)
                    {
                        first_state->addTransition(static_cast<char>(i), state);
                    }
                }
            }
            else
            {
                // É uma transição posterior
                State *last_state = states_list.back();
                last_state->addTransition('\0', state);
                for (const auto &interval : this->getIntervals(action.str))
                {
                    for (int i = std::get<0>(interval); i < std::get<1>(interval); ++i)
                    {
                        last_state->addTransition(static_cast<char>(i), state);
                    }
                }
            }
            states_list.push_back(state);
            break;
        case KLEENE_INTERVAL_SET:
            state = this->createNewState();
            if (states_list.empty())
            {
                // É a primeira transição
                first_state->addTransition('\0', state);
            }
            else
            {
                // É uma transição posterior
                State *last_state = states_list.back();
                last_state->addTransition('\0', state);
            }
            for (const auto &interval : this->getIntervals(action.str))
            {
                for (int i = std::get<0>(interval); i < std::get<1>(interval); ++i)
                {
                    state->addTransition(static_cast<char>(i), state);
                }
            }
            states_list.push_back(state);
            break;
        case PLUS_INTERVAL_SET:
            state = this->createNewState();
            if (states_list.empty())
            {
                // É a primeira transição
                for (const auto &interval : this->getIntervals(action.str))
                {
                    for (int i = std::get<0>(interval); i < std::get<1>(interval); ++i)
                    {
                        first_state->addTransition(static_cast<char>(i), state);
                        state->addTransition(static_cast<char>(i), state);
                    }
                }
            }
            else
            {
                // É uma transição posterior
                State *last_state = states_list.back();
                for (const auto &interval : this->getIntervals(action.str))
                {
                    for (int i = std::get<0>(interval); i < std::get<1>(interval); ++i)
                    {
                        last_state->addTransition(static_cast<char>(i), state);
                        state->addTransition(static_cast<char>(i), state);
                    }
                }
            }
            states_list.push_back(state);
            break;
        case INTERVAL_SET:
            state = this->createNewState();
            if (states_list.empty())
            {
                // É a primeira transição
                for (const auto &interval : this->getIntervals(action.str))
                {
                    for (int i = std::get<0>(interval); i < std::get<1>(interval); ++i)
                    {
                        first_state->addTransition(static_cast<char>(i), state);
                    }
                }
            }
            else
            {
                // É uma transição posterior
                State *last_state = states_list.back();
                for (const auto &interval : this->getIntervals(action.str))
                {
                    for (int i = std::get<0>(interval); i < std::get<1>(interval); ++i)
                    {
                        last_state->addTransition(static_cast<char>(i), state);
                    }
                }
            }
            states_list.push_back(state);
            break;
        case OPTION_SET:
            state = this->createNewState();
            for (char c : action.str)
            {
                if (states_list.empty())
                {
                    // É a primeira transição
                    first_state->addTransition(c, state);
                    first_state->addTransition('\0', state);
                }
                else
                {
                    // É uma transição posterior
                    State *last_state = states_list.back();
                    last_state->addTransition(c, state);
                    last_state->addTransition('\0', state);
                }
            }
            states_list.push_back(state);
            break;
        case KLEENE_SET:
            state = this->createNewState();
            for (char c : action.str)
            {
                if (states_list.empty())
                {
                    // É a primeira transição
                    first_state->addTransition('\0', state);
                    state->addTransition(c, state);
                }
                else
                {
                    // É uma transição posterior
                    State *last_state = states_list.back();
                    last_state->addTransition('\0', state);
                    state->addTransition(c, state);
                }
            }
            states_list.push_back(state);
            break;
        case PLUS_SET:
            state = this->createNewState();
            for (char c : action.str)
            {
                if (states_list.empty())
                {
                    // É a primeira transição
                    first_state->addTransition(c, state);
                    state->addTransition(c, state);
                }
                else
                {
                    // É uma transição posterior
                    State *last_state = states_list.back();
                    last_state->addTransition(c, state);
                    state->addTransition(c, state);
                }
            }
            states_list.push_back(state);
            break;
        case SET:
            state = this->createNewState();
            for (char c : action.str)
            {
                if (states_list.empty())
                {
                    // É a primeira transição
                    first_state->addTransition(c, state);
                }
                else
                {
                    // É uma transição posterior
                    State *last_state = states_list.back();
                    last_state->addTransition(c, state);
                }
            }
            states_list.push_back(state);
            break;
        default:
            std::cout << "Ação com Tipo Inválido";
            exit(1);
            break;
        }
        states_list.clear();
    }
    this->inicial->addTransition('\0', first_state); // Transição Lambda
}

State* GenericAutomata::createNewState()
{
    
    int novoEstadoNum = estados.size() + 1;
    estados.emplace_back(novoEstadoNum); 
    if (inicial == nullptr)
    {
        inicial = &estados.back();
    }
    return &estados.back();
}

std::list<State*> GenericAutomata::toList()
{
    std::list<State*> estadoList;
    for (auto& estado : estados)
    {
        estadoList.push_back(&estado);
    }
    // Ordena a lista pelo número do estado
    estadoList.sort([](State* a, State* b)
                    { return a->getEstado() < b->getEstado(); });
    return estadoList;
}

State* GenericAutomata::findState(int estadoNum) const {
    for (const State& estado : estados) {
        if (estado.getEstado() == estadoNum) {
            return const_cast<State*>(&estado); // Retorna um ponteiro não-const para o estado
        }
    }
    return nullptr; // Retorna nullptr se o estado não for encontrado
}

std::list<Action> GenericAutomata::decodifyRegularExpression(const std::string &re)
{
    std::list<Action> actions;
    std::string aux;
    bool set = false;
    bool interval = false;
    char last_char = '\0';
    for (char c : re)
    {
        switch (c)
        {
        case '[':
            // Inicia o set
            set = true;
            if (aux.length() > 0)
            {
                // Salva a raw string
                actions.push_back({RAW_STRING, aux});
                aux.clear();
            }
            break;

        case '-':
            if (set)
            {
                if (last_char != '[')
                {
                    // Inicia o intervalo
                    interval = true;
                }
            }
            aux += c;
            break;

        case ']':
            // Termina um set ou um intervalo
            if (set)
            {
                if (interval)
                {
                    // Salva o intervalo
                    actions.push_back({INTERVAL_SET, aux});
                    aux.clear();
                }
                else
                {
                    // Salva o set
                    actions.push_back({SET, aux});
                    aux.clear();
                }
                set = false;
                interval = false;
            }
            break;

        case '+':
            if (last_char == ']')
            {
                if (actions.back().tipo == INTERVAL_SET)
                {
                    actions.back().tipo = PLUS_INTERVAL_SET;
                }
                else
                {
                    actions.back().tipo = PLUS_SET;
                }
            }
            else
            {
                aux += c;
            }
            break;

        case '*':
            if (last_char == ']')
            {
                if (actions.back().tipo == INTERVAL_SET)
                {
                    actions.back().tipo = KLEENE_INTERVAL_SET;
                }
                else
                {
                    actions.back().tipo = KLEENE_SET;
                }
            }
            else
            {
                aux += c;
            }
            break;
        case '?':
            if (last_char == ']')
            {
                if (actions.back().tipo == INTERVAL_SET)
                {
                    actions.back().tipo = OPTION_INTERVAL_SET;
                }
                else
                {
                    actions.back().tipo = OPTION_SET;
                }
            }
            else
            {
                aux += c;
            }
            break;

        default:
            aux += c;
            break;
        }
        last_char = c;
    }
    if (aux.length() > 0)
    {
        // Salva a raw string
        actions.push_back({RAW_STRING, aux});
        aux.clear();
    }
    return actions;
}

std::list<std::tuple<int, int>> GenericAutomata::getIntervals(const std::string &intervals)
{
    std::list<std::tuple<int, int>> intervalos;

    for (size_t i = 0; i < intervals.length(); i += 3)
    {
        if (i + 2 < intervals.length() && intervals[i + 1] == '-')
        {
            int inicio = static_cast<int>(intervals[i]);
            int fim = static_cast<int>(intervals[i + 2]);
            intervalos.push_back(std::make_tuple(inicio, fim));
        }
    }
    return intervalos;
}
