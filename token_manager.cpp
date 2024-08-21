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
        final_state_map[token_id].insert(final_state);
        return true;
    }
    return false;
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
    for (const auto &pair : token_map)
    {
        all_tokens[pair.first] = getFinalStates(pair.first);
    }
    return all_tokens;
}

std::string TokenManager::getTokenByFinalState(int final_state) const
{
    for (const auto &pair : final_state_map)
    {
        if (pair.second.find(final_state) != pair.second.end())
        {
            for (const auto &token_pair : token_map)
            {
                if (token_pair.second == pair.first)
                {
                    return token_pair.first;
                }
            }
        }
    }
    return "";
}

bool TokenManager::isFinalState(int state) const
{
    for (const auto &pair : final_state_map)
    {
        if (pair.second.find(state) != pair.second.end())
        {
            return true;
        }
    }
    return false;
}