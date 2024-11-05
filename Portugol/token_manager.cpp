#include "token_manager.hpp"

TokenManager::TokenManager() : next_id(1) {}

int TokenManager::addToken(const std::string &token_name)
{
    if (token_map.find(token_name) == token_map.end())
    {
        token_map[token_name] = next_id++;
    }
    return token_map[token_name];
}

std::string TokenManager::getToken(int token_id) const
{
    for (const auto &[name, id] : token_map)
    {
        if (id == token_id)
        {
            return name;
        }
    }
    return ""; // Retorna uma string vazia se o ID não for encontrado
}

std::string TokenManager::getTokenByFinalState(int final_state) const
{
    auto it = state_to_token_map.find(final_state);
    if (it != state_to_token_map.end())
    {
        return getToken(it->second);
    }
    return ""; // Retorna uma string vazia se o estado não for final
}

int TokenManager::getTokenId(const std::string &token_name) const
{
    auto it = token_map.find(token_name);
    if (it != token_map.end())
    {
        return it->second;
    }
    return -1; // Token não encontrado
}

int TokenManager::getTokenIdByFinalState(int final_state) const
{
    auto it = state_to_token_map.find(final_state);
    if (it != state_to_token_map.end())
    {
        return it->second;
    }
    return -1; // Estado não encontrado
}

std::vector<int> TokenManager::getFinalStates(const std::string &token_name) const
{
    int token_id = getTokenId(token_name);
    std::vector<int> states;
    if (token_id != -1)
    {
        auto it = final_state_map.find(token_id);
        if (it != final_state_map.end())
        {
            states.assign(it->second.begin(), it->second.end());
        }
    }
    return states;
}

std::unordered_map<std::string, std::vector<int>> TokenManager::getAllTokens() const
{
    std::unordered_map<std::string, std::vector<int>> all_tokens;
    for (const auto &[token_name, token_id] : token_map)
    {
        all_tokens[token_name] = getFinalStates(token_name);
    }
    return all_tokens;
}

bool TokenManager::setFinalState(const std::string &token_name, int final_state)
{
    int token_id = getTokenId(token_name);
    if (token_id != -1)
    {
        final_state_map[token_id].insert(final_state);
        state_to_token_map[final_state] = token_id;
        return true;
    }
    return false;
}

bool TokenManager::isFinalState(int state) const
{
    return state_to_token_map.find(state) != state_to_token_map.end();
}

void TokenManager::removeFinalState(int final_state)
{
    // Verifica se o estado final existe
    auto it = state_to_token_map.find(final_state);
    if (this->isFinalState(final_state))
    {
        // Obtém o token ID associado ao estado final
        int token_id = it->second;

        // Remove o estado final do mapa de estados finais para o token correspondente
        auto final_state_it = final_state_map.find(token_id);
        if (final_state_it != final_state_map.end())
        {
            final_state_it->second.erase(final_state);

            // Se não houver mais estados finais associados ao token, remova a entrada do mapa
            if (final_state_it->second.empty())
            {
                final_state_map.erase(final_state_it);
            }
        }

        // Remove o estado final do mapa state_to_token_map
        state_to_token_map.erase(it);
    }
}

void TokenManager::replaceFinalStateMap(std::unordered_map<int, std::unordered_set<int>> new_map)
{
    final_state_map = std::move(new_map);
    // Remove os múltiplos estados finais se houver, mantendo a consistência dos dados
    this->reduceFinalStateMap();
    state_to_token_map.clear();

    // Reconstituir o mapa state_to_token_map com base no novo final_state_map
    for (const auto &[token_id, states] : final_state_map)
    {
        for (int state : states)
        {
            state_to_token_map[state] = token_id;
        }   
    }
}

void TokenManager::reduceFinalStateMap()
{
    std::unordered_map<int, int> estado_para_token; // Mapeia estados finais para o token_id com a menor prioridade

    for (auto &[token_id, estados_finais] : final_state_map)
    {
        std::unordered_set<int> estados_a_remover;

        for (int estado : estados_finais)
        {
            auto it = estado_para_token.find(estado);

            if (it == estado_para_token.end())
            {
                // Se o estado não está mapeado, adiciona-o ao mapa
                estado_para_token[estado] = token_id;
            }
            else if (it->second > token_id)
            {
                // Se o estado está mapeado para um token_id maior, atualiza o token_id no mapa e marca para remoção
                final_state_map[it->second].erase(estado);
                estado_para_token[estado] = token_id;
            }
            else
            {
                // Se o estado já está mapeado para um token_id menor ou igual, marca para remoção
                estados_a_remover.insert(estado);
            }
        }

        // Remove os estados que foram duplicados em tokens de menor prioridade
        for (int estado : estados_a_remover)
        {
            estados_finais.erase(estado);
        }
    }
}
