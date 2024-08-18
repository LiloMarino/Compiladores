#ifndef AUTOMATO_FINITO_HPP
#define AUTOMATO_FINITO_HPP

#include <string>

class AutomatoFinito
{
private:
    int num_estados; // Linhas da matriz
    int **matriz;

public:
    /**
     * @brief Construtor de Autômato
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
};

#endif
