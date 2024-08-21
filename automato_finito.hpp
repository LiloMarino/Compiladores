#ifndef AUTOMATO_FINITO_HPP
#define AUTOMATO_FINITO_HPP

#include <string>
#include "generic_automata.hpp"
#include "token_manager.hpp"

/**
 * @brief Classe de Autômato Finito
 * @author Murilo Marino
 */
class AutomatoFinito
{
private:
    int num_estados; // Linhas da matriz
    int **matriz;
    bool deterministico;
    GenericAutomata *afnd;
    TokenManager tokens;
public:
    /**
     * @brief Construtor do Autômato Finito Não Determinístico
     */
    AutomatoFinito();

    /**
     * @brief Construtor do Autômato Finito Determinístico
     * @param num_estados Número de estados do autômato
     */
    AutomatoFinito(const int num_estados);

    /**
     * @brief Destrutor do autômato
     */
    ~AutomatoFinito();

    /**
     * @brief Adiciona as transições entre o estado inicial e o estado final
     * @param estado_inicial Número do estado inicial
     * @param estado_final Número do estado final
     * @param transitions Transições separadas por vírgula podendo ter o '-' definindo um intervalo
     * Por exemplo: 0-9 ele irá entender como todos os números de 0 até 9
     * Outro exemplo: a,b,0-9 ele irá entender como todos os números de 0 até 9 e também as letras 'a' e 'b'
     */
    void addTransitions(const int estado_inicial, const int estado_final, const std::string &transitions);

    /**
     * @brief Implementa uma expressão regular no autômato
     * @param re Expressão regular
     * @param token Token reconhecido
     */
    void addRegularExpression(const std::string &re, const std::string &token);

    /**
     * @brief Aplica o algoritmo para transformar o AFND-e em AFD
     */
    void toAFD();

    /**
     * @brief Aplica o algoritmo para transformar o AFND-e em AFND
     */
    void toAFND();

    /**
     * @brief Faz a transição dentro do autômato a partir do estado atual para
     * um estado de destino determinado pela letra lida
     *
     * @param estado_atual Estado atual
     * @param letra Letra lida
     * @return Estado destino determinado pelo autômato
     */
    int makeTransition(const int estado_atual, const char letra);

    /**
     * @brief Getter do número de estados
     * @return Retorna o número de estados
     */
    int getNumEstados();

    /**
     * @brief Imprime a tabela de transições do Autômato
     * @param output Output de saída da tabela
     */
    void printTransitionTable(std::ostream &output);

    /**
     * @brief Cria um arquivo DOT representando o autômato finito
     * @param filename Nome do arquivo dot
     */
    void printVisualizacaoDOT(const std::string &filename);

    /**
     * @brief Polimorfismo do operador '==', compara 2 autômatos finitos e diz se eles são equivalentes
     * @param outro Outro Autômato Finito Determinístico
     * @return Retorna true se foram equivalentes e falso caso contrário
     * @warning Só é possível comparar Autômatos Finitos se eles forem determinísticos
     */
    bool operator==(const AutomatoFinito &outro) const;

private:
    /**
     * @brief Para cada transição lambda deste estado
     * obtém todas as transições terminais de seus filhos-lambda
     * e se seus filhos-lambda possuirem transições lambda 
     * então ele também irá obter as transições terminais dos filhos 
     * de seus filhos-lambda e assim sucessivamente resultando
     * em um novo conjunto de transições para este estado
     * @param estado Estado que será eliminado as transições lambda
     * @return Retorna o novo vetor de tuplas das transições para esse estado 
     * com as transições lambdas eliminadas 
     * <int, char, int> são [origem, entrada, destino] respectivamente
     */
    std::vector<std::tuple<int, char, int>> resolveLambdaTransitions(State* estado);

    /**
     * @brief Transpõe para a estrutura de matriz o elemento afnd
     */
    void transposeAFD();
    
};

#endif
