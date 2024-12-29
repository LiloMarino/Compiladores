#ifndef MIPS_HPP
#define MIPS_HPP

#include "expression.hpp"
#include <queue>

#define TEMPORARY_REGISTER 10
#define SAVE_REGISTER 8

class MIPS
{
private:
    std::queue<std::string> data;
    std::queue<std::string> text;
    bool temp_registers[TEMPORARY_REGISTER];
    bool save_registers[SAVE_REGISTER];
    int string_count = 0;
    int if_count = 0;
    int while_count = 0;
    int for_count = 0;

public:
    /**
     * @brief Construtor do MIPS
     */
    MIPS();

    /**
     * @brief Obtém o índice de um registrador temporário e marca ele como em uso
     * @return Índice do registrador temporário
     */
    int getTemporaryRegister();

    /**
     * @brief Obtém o índice de um registrador salvo e marca ele como em uso
     * @return Índice do registrador salvo
     */
    int getSaveRegister();

    /**
     * @brief Marca o registrador temporário como livre
     * @param index Índice do registrador temporário
     */
    void freeTemporaryRegister(const int index);

    /**
     * @brief Marca o registrador salvo como livre
     * @param index Índice do registrador salvo
     */
    void freeSaveRegister(const int index);

    /**
     * @brief Obtém o nome do registrador
     * @param index Índice do registrador
     * @return Nome do registrador
     */
    std::string getRegisterName(const int index);

    /**
     * @brief Imprime o código MIPS
     */
    void printCode();

    /**
     * @brief Cria uma constante em MIPS
     * @param constant Valor da constante
     */
    void createConstant(const int constant);

    /**
     * @brief Cria uma string em MIPS
     * @param string Valor da string
     * @return Label da string
     */
    std::string createString(const std::string &string);

    /**
     * @brief Cria uma variável global em MIPS
     * @param identifier Nome da variável
     * @param value Valor da variável
     */
    void createGlobalVar(const std::string &identifier, const int value);

    /**
     * @brief Traduz uma expressão para MIPS
     * @param op Operador da expressão
     * @param r1 Índice do primeiro registrador
     * @param r2 Índice do segundo registrador
     */
    void createExpression(const OperatorType op, const int r1, const int r2);

    /**
     * @brief Traduz uma chamada de função para MIPS
     * @param function_name Nome da função
     */
    void callFunction(const std::string &function_name);

    /**
     * @brief Imprime uma string
     * @param string String a ser impressa
     */
    void callPrintf(const std::string &string);

    /**
     * @brief Imprime um valor inteiro
     * @param value Valor a ser impresso
     */
    void callPrintf(const int value);

    /**
     * @brief Lê um valor inteiro e salva na variável
     * @param identifier Nome da variável
     */
    void callScanf(const std::string &identifier);
    
    /**
     * @brief Sai do programa com o status de saída
     * @param status Status de saída
     */
    void callExit(const int status);
};

#endif