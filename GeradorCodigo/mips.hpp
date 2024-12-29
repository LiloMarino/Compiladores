#ifndef MIPS_HPP
#define MIPS_HPP

#include "expression.hpp"

#define TEMPORARY_REGISTER 10
#define SAVE_REGISTER 8

class MIPS
{
private:
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
    void freeTemporaryRegister(int index);

    /**
     * @brief Marca o registrador salvo como livre
     * @param index Índice do registrador salvo
     */
    void freeSaveRegister(int index);

    /**
     * @brief Obtém o nome do registrador
     * @param index Índice do registrador
     * @return Nome do registrador
     */
    std::string getRegisterName(int index);

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
     * @brief
     * @param op
     * @param r1
     * @param r2
     */
    void printExpression(OperatorType op, const int r1, const int r2);

    void callFunction();
    void callPrintf();
    void callScanf();
    void callExit();
};

#endif