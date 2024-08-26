#ifndef TOKEN_MANAGER_HPP
#define TOKEN_MANAGER_HPP

#include <unordered_map>
#include <unordered_set>
#include <string>
#include <vector>

/**
 * @class TokenManager
 * @brief Gerencia tokens, seus identificadores únicos e estados finais.
 * @note Se o Id do Token não for utilizado é melhor otimizar a classe para 2 maps
 * std::unordered_map<std::string, std::unordered_set<int>> token_map;
 * std::unordered_map<int, std::string> final_to_token_map;
 */
class TokenManager
{
private:
    std::unordered_map<std::string, int> token_map;                   // Token -> TokenId
    std::unordered_map<int, std::unordered_set<int>> final_state_map; // TokenId -> Final State
    std::unordered_map<int, int> state_to_token_map;                  // Final State -> TokenId
    int next_id;

public:
    /**
     * @brief Construtor padrão que inicializa o próximo identificador disponível.
     */
    TokenManager();

    /**
     * @brief Adiciona um novo token ao gerenciador.
     * @param token_name O nome do token a ser adicionado.
     * @return O identificador único do token.
     */
    int addToken(const std::string &token_name);

    /**
     * @brief Obtém o identificador de um token.
     * @param token_id Id do token a ser obtido o identificador
     * @return Nome do token
     */
    std::string getToken(int token_id) const;

    /**
     * @brief Obtém o token associado a um estado final específico.
     * @param final_state O estado final para o qual se deseja obter o token.
     * @return O nome do token associado ao estado final ou uma string vazia se nenhum token for encontrado.
     */
    std::string getTokenByFinalState(int final_state) const;

    /**
     * @brief Obtém o identificador de um token existente.
     * @param token_name O nome do token.
     * @return O identificador do token ou -1 se o token não for encontrado.
     */
    int getTokenId(const std::string &token_name) const;

    /**
     * @brief Obtém o id do token associado a um estado final específico.
     * @param final_state O estado final para o qual se deseja obter o token.
     * @return O id do token associado ao estado final ou -1 se o estado não for final.
     */
    int getTokenIdByFinalState(int final_state) const;

    /**
     * @brief Obtém todos os estados finais associados a um token.
     * @param token_name O nome do token.
     * @return Um vetor com todos os estados finais do token ou um vetor vazio se o token não for encontrado.
     */
    std::vector<int> getFinalStates(const std::string &token_name) const;

    /**
     * @brief Obtém todos os tokens e seus estados finais.
     * @return Um mapa onde a chave é o nome do token e o valor é um vetor de estados finais.
     */
    std::unordered_map<std::string, std::vector<int>> getAllTokens() const;

    /**
     * @brief Associa um estado final a um token.
     * @param token_name O nome do token.
     * @param final_state O estado final a ser associado.
     * @return true se a associação foi bem-sucedida, false se o token não foi encontrado.
     */
    bool setFinalState(const std::string &token_name, int final_state);

    /**
     * @brief Verifica se um estado específico é um estado final.
     * @param state O estado a ser verificado.
     * @return true se o estado for final, false caso contrário.
     */
    bool isFinalState(int state) const;

    /**
     * @brief Remove o estado final
     * @param final_state Número do estado final
     */
    void removeFinalState(int final_state);

    /**
     * @brief Sobrescreve o mapa de estados finais
     * @param new_map Novo mapa de estados finais
     */
    void replaceFinalStateMap(std::unordered_map<int, std::unordered_set<int>> new_map);

private:
    /**
     * @brief Remove os múltiplos estados finais de `final_state_map` é útil especialmente após o replace
     * garantindo a consistência dos dados
     * @note Ele usa como base o TokenId para isso, portanto quem foi declarado primeiro e por consequência
     * tem o menor TokenId tem a maior prioridade, o estado final então passará a reconhecer o TokenId de maior
     * prioridade, seguindo essa regra
     */
    void reduceFinalStateMap();
};

#endif