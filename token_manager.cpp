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
    int token_id = getTokenIdByFinalState(final_state);
    if (token_id != -1)
    {
        return getToken(token_id);
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
    for (const auto &[token_id, estados_finais] : final_state_map)
    {
        if (estados_finais.find(final_state) != estados_finais.end())
        {
            return token_id;
        }
    }
    return -1; // Token não encontrado
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
        return true;
    }
    return false;
}

bool TokenManager::isFinalState(int state) const
{
    for (const auto &[token_id, estados_finais] : final_state_map)
    {
        if (estados_finais.find(state) != estados_finais.end())
        {
            return true;
        }
    }
    return false;
}

void TokenManager::replaceFinalStateMap(std::unordered_map<int, std::unordered_set<int>> new_map)
{
    final_state_map = std::move(new_map);
}
