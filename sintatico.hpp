#ifndef SINTATICO_HPP
#define SINTATICO_HPP

#include "lexico.hpp"
#include <stack>

// Estrutura que representa uma produção na tabela LL(1)
struct SintaticGroup
{
    std::string pop;
    std::string push;

    // Construtor padrão
    SintaticGroup() : pop(""), push("") {}

    // Construtor com parâmetros
    SintaticGroup(const std::string &p_pop, const std::string &p_push)
        : pop(p_pop), push(p_push) {}

    // Método para verificar se a produção é vazia
    bool isEmpty() const
    {
        return pop.empty() && push.empty();
    }
};

class AnalisadorSintatico
{
private:
    std::string simbolo_inicial;
    std::unordered_map<std::string, int> nao_terminais;    // Mapeamento de não terminais para índices
    std::unordered_map<std::string, int> terminais;        // Mapeamento de terminais/tokens para índices
    std::vector<std::vector<SintaticGroup>> parsing_table; // Tabela LL(1)
public:
    /**
     * @brief Construtor do AnalisadorSintatico
     * @param simbolo_inicial Primeiro não terminal da gramática
     */
    AnalisadorSintatico(std::string simbolo_inicial);

    /**
     * @brief Adiciona uma produção a tabela
     * @param origem Não terminal de origem
     * @param token Token a ser obtido
     * @param produto Cadeia produzida pela derivação
     * @note Quero ler `token` então `origem` => `produto`
     */
    void addProduction(const std::string &origem, const std::string &token, const std::string &produto);

    /**
     * @brief Realiza uma derivação na tabela
     * @param nao_terminal Não terminal a ser derivado
     * @param terminal Terminal desejado
     * @return Grupo Sintático para operar a pilha
     */
    SintaticGroup getProduction(const std::string &nao_terminal, const std::string &terminal) const;

    /**
     * @brief Adiciona um não terminal a tabela
     * @param nao_terminal Não terminal a ser adicionado
     */
    void adicionarNaoTerminal(const std::string &nao_terminal);

    /**
     * @brief Adiciona um terminal a tabela
     * @param terminal Terminal a ser adicionado
     */
    void adicionarTerminal(const std::string &terminal);

    /**
     * @brief Reconhece a sequência de tokens da entrada e verifica se ela pertence ou não a linguagem
     * @param entrada Sequência de tokens reconhecidos pelo AnalisadorLexico
     * @return Verdadeiro se pertence e falso caso não pertença a linguagem
     */
    void analisar(const std::list<LexicalGroup> &entrada);

    /**
     * @brief Printa a tabela LL(1)
     */
    void exibirTabela(std::ostream &output) const;

    /**
     * @brief Exceção do AnalisadorSintático
     */
    class SintaticError : public std::exception
    {
    private:
        std::string mensagem;

    public:
        explicit SintaticError(const std::string &msg) : mensagem(msg) {}

        virtual const char *what() const noexcept override;
    };
};

#endif