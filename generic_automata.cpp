#include "generic_automata.hpp"
#include <iostream>

using namespace std;

GenericAutomata::GenericAutomata() : total_estados(1), inicial(1)
{
}

GenericAutomata::~GenericAutomata()
{
    // Usando o iterador para percorrer todos os estados
    for (auto it = begin(); it != end(); ++it)
    {
        State *state = &(*it);
        // Remove o estado da memória
        delete state;
    }
}


void GenericAutomata::addRegularExpression(const std::string &re)
{
    list<Action> actions = this->decodifyRegularExpression(re);
    list<State *> states_list;
    State *first_state = new State(++this->total_estados);
    for (auto action : actions)
    {
        State *state; // Variável auxiliar
        switch (action.tipo)
        {
        case RAW_STRING:
            for (char c : action.str)
            {
                state = new State(++this->total_estados);
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
            break;
        case KLEENE_INTERVAL_SET:
            break;
        case PLUS_INTERVAL_SET:
            break;
        case INTERVAL_SET:
            break;
        case OPTION_SET:
            break;
        case KLEENE_SET:
            break;
        case PLUS_SET:
            break;
        case SET:
            state = new State(++this->total_estados);
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
            cout << "Ação com Tipo Inválido";
            exit(1);
            break;
        }
        states_list.clear();
    }
    this->inicial.addTransition('\0', first_state); // Transição Lambda
}

list<Action> GenericAutomata::decodifyRegularExpression(const string &re)
{
    list<Action> actions;
    string aux;
    bool set = false;
    bool interval = false;
    char last_char;
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

std::list<std::tuple<int, int>> GenericAutomata::getIntervals(const std::string &intervals) {
    list<tuple<int, int>> intervalos;

    for (size_t i = 0; i < intervals.length(); i += 3) {
        if (i + 2 < intervals.length() && intervals[i + 1] == '-') {
            int inicio = static_cast<int>(intervals[i]);
            int fim = static_cast<int>(intervals[i + 2]);
            intervalos.push_back(make_tuple(inicio, fim));
        }
    }
    return intervalos;
}

// Implementação do construtor do iterador
GenericAutomata::Iterator::Iterator(State *root)
{
    if (root != nullptr)
    {
        stack.push(root);
        visited.insert(root);
    }
}

// Operador de desigualdade para comparação com end()
bool GenericAutomata::Iterator::operator!=(const Iterator &other) const
{
    return !stack.empty();
}

// Operador de dereferência para acessar o estado atual
State &GenericAutomata::Iterator::operator*()
{
    return *stack.top();
}

// Operador de incremento para mover para o próximo estado
GenericAutomata::Iterator &GenericAutomata::Iterator::operator++()
{
    if (stack.empty())
    {
        return *this;
    }

    State *current = stack.top();
    stack.pop();

    // Adiciona as transições do estado atual à pilha, se não tiver sido visitado
    for (const auto &transition : current->getTransitions())
    {
        if (visited.find(transition.estado_destino) == visited.end())
        {
            stack.push(transition.estado_destino);
            visited.insert(transition.estado_destino);
        }
    }
    return *this;
}

GenericAutomata::Iterator GenericAutomata::begin()
{
    return Iterator(&inicial); // Retorna um iterador para o estado inicial
}

GenericAutomata::Iterator GenericAutomata::end()
{
    return Iterator(nullptr); // Iterador de fim (representando o fim da árvore)
}