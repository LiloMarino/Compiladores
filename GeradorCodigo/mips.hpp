#ifndef MIPS_HPP
#define MIPS_HPP

#include "expression.hpp"
#include <queue>

#define ARGUMENT_REGISTER 4
#define TEMPORARY_REGISTER 10
#define SAVE_REGISTER 8

class MIPS
{
private:
    static std::queue<std::string> data;
    static std::queue<std::string> text;
    static bool arg_registers[ARGUMENT_REGISTER];
    static bool temp_registers[TEMPORARY_REGISTER];
    static bool save_registers[SAVE_REGISTER];
    static int string_count;
    static std::pair<int, int> if_count;
    static std::pair<int, int> while_count;
    static std::pair<int, int> for_count;

public:
    /**
     * @brief Inicializador do MIPS
     */
    static void initialize();

    /**
     * @brief Obtém o índice de um registrador argumento e marca ele como em uso
     * @return Índice do registrador argumento
     */
    static int getArgumentRegister();

    /**
     * @brief Obtém o índice de um registrador temporário e marca ele como em uso
     * @return Índice do registrador temporário
     */
    static int getTemporaryRegister();

    /**
     * @brief Obtém o índice de um registrador salvo e marca ele como em uso
     * @return Índice do registrador salvo
     */
    static int getSaveRegister();

    /**
     * @brief Marca o registrador argumento como livre
     * @param index Índice do registrador argumento
     */
    static void freeArgumentRegister(const int index);

    /**
     * @brief Marca o registrador temporário como livre
     * @param index Índice do registrador temporário
     */
    static void freeTemporaryRegister(const int index);

    /**
     * @brief Marca o registrador salvo como livre
     * @param index Índice do registrador salvo
     */
    static void freeSaveRegister(const int index);

    /**
     * @brief Obtém o nome do registrador
     * @param index Índice do registrador
     * @return Nome do registrador
     */
    static std::string getRegisterName(const int index);

    /**
     * @brief Imprime o código MIPS
     */
    static void printCode();

    /**
     * @brief Cria uma constante em MIPS
     * @param identifier Identificador da constante
     * @param constant Valor da constante
     */
    static void createConstant(const std::string &identifier, const int constant);

    /**
     * @brief Cria uma string em MIPS
     * @param string Valor da string
     * @return Label da string
     */
    static std::string createString(const std::string &string);

    /**
     * @brief Cria uma variável global em MIPS
     * @param identifier Nome da variável
     * @param value Valor da variável
     */
    static void createGlobalVar(const std::string &identifier, const int value);

    /**
     * @brief Traduz uma expressão para MIPS
     * @param op Operador da expressão
     * @param r1 Índice do primeiro registrador
     * @param r2 Índice do segundo registrador
     */
    static void createExpression(const OperatorType op, const int r1, const int r2);

    /**
     * @brief Inicia um loop de while
     * @return Label do while
     */
    static std::string startWhile();

    /**
     * @brief Obtém a label do fim do while atual
     * @return Label do fim do while
     */
    static std::string getEndWhile();

    /**
     * @brief Finaliza um loop de while
     */
    static void endWhile();

    /**
     *
     * @brief Inicia um if
     */
    static void startIf();

    /**
     * @brief Obtém a label do else do if atual
     * @return Label do else
     */
    static std::string getElse();

    /**
     * @brief Inicia um else
     */
    static void startElse();

    /**
     * @brief Obtém a label do fim do if atual
     * @return Label do fim do if
     */
    static std::string getEndIf();

    /**
     * @brief Finaliza um if
     */
    static void endIf();

    /**
     * @brief Inicia um loop de for
     * @return Label do for
     */
    static std::string startFor();

    /**
     * @brief Obtém a label do fim do for atual
     * @return Label do fim do for
     */
    static std::string getEndFor();

    /**
     * @brief Finaliza um loop de for
     */
    static void endFor();

    /**
     * @brief Cria uma label
     * @param label Nome da label
     */
    static void createLabel(const std::string &label);

    /**
     * @brief Faz um salto para uma label
     * @param label Nome da label
     */
    static void jumpTo(const std::string &label);

    /**
     * @brief Traduz uma chamada de função para MIPS
     * @param function_name Nome da função
     */
    static void callFunction(const std::string &function_name);

    /**
     * @brief Imprime uma string
     * @param string String a ser impressa
     */
    static void callPrintf(const std::string &string);

    /**
     * @brief Imprime um valor inteiro
     * @param value Valor a ser impresso
     */
    static void callPrintf(const int value);

    /**
     * @brief Imprime uma string a partir de um registrador
     * @param rg Registrador com endereço da string
     */
    static void callPrintfAsString(const int rg);

    /**
     * @brief Imprime o caractere
     * @param rg Registrador com caractere
     */
    static void callPrintfAsChar(const int rg);

    /**
     * @brief Lê um valor inteiro e salva na variável
     * @param identifier Nome da variável
     */
    static void callScanf(const std::string &identifier);

    /**
     * @brief Retorna a função com o valor de um registrador
     * @param rg Registrador com valor a ser retornado
     * @note O registrador -1 indica que não retorna nada
     */
    static void callReturn(const int rg);

    /**
     * @brief Sai do programa com o status de saída
     * @param rg Registrador com status de saída
     */
    static void callExit(const int rg);
};

#endif