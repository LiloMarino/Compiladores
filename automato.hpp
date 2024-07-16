#ifndef AUTOMATO_HPP
#define AUTOMATO_HPP

#include <string>


class Automato
{
private:
    int num_estados; // Linhas da matriz
    int **matriz;

public:
    /**
     * @brief Construtor de Autômato
     * @param num_estados Número de estados do autômato
     */
    Automato(const int num_estados);

    /**
     * @brief Destrutor do autômato
     */
    ~Automato();

    /**
     * @brief Adiciona as transições entre o estado inicial e o estado final
     * @param estado_inicial Número do estado inicial
     * @param estado_final Número do estado final
     * @param transitions Transições separadas por vírgula podendo ter o '-' definindo um intervalo
     * Por exemplo: 0-9 ele irá entender como todos os números de 0 até 9
     * Outro exemplo: a,b,0-9 ele irá entender como todos os números de 0 até 9 e também as letras 'a' e 'b'
     */
    void addTransition(const int estado_inicial, const int estado_final, const std::string& transitions);
};

#endif
