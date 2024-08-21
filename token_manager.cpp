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

int TokenManager::getTokenId(const std::string &token_name) const
{
    auto it = token_map.find(token_name);
    if (it != token_map.end())
    {
        return it->second;
    }
    return -1; // Token não encontrado
}

bool TokenManager::setFinalState(const std::string &token_name, int final_state)
{
    int token_id = getTokenId(token_name);
    if (token_id != -1)
    {
        final_state_map[token_id] = final_state;
        return true;
    }
    return false;
}

int TokenManager::getFinalState(const std::string &token_name) const
{
    int token_id = getTokenId(token_name);
    if (token_id != -1)
    {
        auto it = final_state_map.find(token_id);
        if (it != final_state_map.end())
        {
            return it->second;
        }
    }
    return -1; // Token ou estado final não encontrado
}
