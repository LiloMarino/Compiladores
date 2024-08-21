#ifndef TOKEN_MANAGER_HPP
#define TOKEN_MANAGER_HPP

#include <unordered_map>
#include <unordered_set>
#include <string>
#include <vector>

/**
 * @class TokenManager
 * @brief Gerencia tokens, seus identificadores únicos e estados finais.
 */
class TokenManager
{
private:
    std::unordered_map<std::string, int> token_map;
    std::unordered_map<int, std::unordered_set<int>> final_state_map;
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
     * @brief Obtém o identificador de um token existente.
     * @param token_name O nome do token.
     * @return O identificador do token ou -1 se o token não for encontrado.
     */
    int getTokenId(const std::string &token_name) const;

    /**
     * @brief Associa um estado final a um token.
     * @param token_name O nome do token.
     * @param final_state O estado final a ser associado.
     * @return true se a associação foi bem-sucedida, false se o token não foi encontrado.
     */
    bool setFinalState(const std::string &token_name, int final_state);


    /**
     * @brief Obtém todos os estados finais associados a um token.
     * @param token_name O nome do token.
     * @return Um vetor com todos os estados finais do token ou um vetor vazio se o token não for encontrado.
     */
    std::vector<int> getFinalStates(const std::string& token_name) const;

    /**
     * @brief Obtém todos os tokens e seus estados finais.
     * @return Um mapa onde a chave é o nome do token e o valor é um vetor de estados finais.
     */
    std::unordered_map<std::string, std::vector<int>> getAllTokens() const;
};

#endif
