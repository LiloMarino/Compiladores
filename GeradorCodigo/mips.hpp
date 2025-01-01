#ifndef MIPS_HPP
#define MIPS_HPP

#include "expression.hpp"
#include <queue>
#include <stack>
#include <functional>

#define ARGUMENT_REGISTER 4
#define TEMPORARY_REGISTER 10
#define SAVE_REGISTER 8
#define RETURN_REGISTER 22

class MIPS
{
private:
    static std::queue<std::string> data;
    static std::queue<std::string> text;
    static bool arg_registers[ARGUMENT_REGISTER];
    static bool temp_registers[TEMPORARY_REGISTER];
    static bool save_registers[SAVE_REGISTER];
    static int string_count;
    static std::pair<int, std::stack<int>> ternary_data;
    static std::pair<int, std::stack<int>> if_data;
    static std::pair<int, std::stack<int>> while_data;
    static std::pair<int, std::stack<int>> for_data;

public:
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
     * @brief Marca todos os registradores como livres
     */
    static void freeAllRegisters();

    /**
     * @brief Obtém o nome do registrador
     * @param index Índice do registrador
     * @return Nome do registrador
     */
    static std::string getRegisterName(const int index);

    /**
     * @brief Obtém o índice do registrador
     * @param name Nome do registrador
     * @return Índice do registrador
     */
    static int getRegisterIndex(const std::string &name);

    /**
     * @brief Verifica se o registrador está em uso
     * @param index Índice do registrador
     * @return True se o registrador está em uso, false caso contrário
     */
    static bool isRegisterInUse(const int index);

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
     * @brief Traduz uma expressão binária para MIPS
     * @param op Operador da expressão
     * @param r1 Índice do primeiro registrador
     * @param r2 Índice do segundo registrador
     * @param rg_result Índice do registrador de resultado
     */
    static void createExpression(const OperatorType op, const int r1, const int r2, const int rg_result);

    /**
     * @brief Traduz uma expressão unária para MIPS
     * @param op Operador da expressão
     * @param r1 Índice do registrador
     * @param rg_result Índice do registrador de resultado
     */
    static void createExpression(const OperatorType op, const int r1, const int rg_result);

    /**
     * @brief Traduz uma expressão de acesso a um array para MIPS
     * @param array_identifier Identificador do array
     * @param rg_index Índice do registrador de índice
     * @param rg_result Índice do registrador de resultado
     */
    static void createArrayAccess(const std::string &array_identifier, const int rg_index, const int rg_result);

    /**
     * @brief Inicializa uma constante em um registrador
     * @param rg_dst Índice do registrador de resultado
     * @param value Valor da constante
     */
    static void initializeConstant(const int rg_dst, const int value);

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
     *
     * @brief Inicia um ternário
     */
    static void startTernary();

    /**
     * @brief Obtém a label do else do ternário atual
     * @return Label do else do ternário
     */
    static std::string getElseTernary();

    /**
     * @brief Inicia um else de um ternário
     */
    static void startElseTernary();

    /**
     * @brief Obtém a label do fim do ternário atual
     * @return Label do fim do ternário
     */
    static std::string getEndTernary();

    /**
     * @brief Finaliza um ternário
     */
    static void endTernary();

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
     * @brief Movimenta o valor do registrador r_src para r_dst
     * @param r_src Registrador de origem
     * @param r_dst Registrador de destino
     */
    static void moveTo(const int r_src, const int r_dst);

    /**
     * @brief Faz um salto para uma label se uma condição for igual a zero
     * @param rg Registrador que contém o resultado da condição
     * @param label Nome da label
     */
    static void branchEqualZero(const int rg, const std::string &label);

    /**
     * @brief Faz um salto para uma label se o registrador for diferente de zero
     * @param rg Registrador que contém o resultado da condição
     * @param label Nome da label
     */
    static void branchNotEqualZero(const int rg, const std::string &label);

    /**
     * @brief Obtém a endereço de uma label
     * @param rg Registrador que irá conter o endereço da label
     * @param label Nome da label
     */
    static void getAddress(const int rg, const std::string &label);

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
     * @brief Lê um valor inteiro e salva no registrador
     * @param rg Registrador a ser salvo
     */
    static void callScanf(const int rg);

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

private:
    /**
     * @brief Cria uma instrução a partir de registradores
     * @param instruction Instrução a ser criada
     * @param r1 Primeiro registrador
     * @param r2 Segundo registrador
     * @param r3 Terceiro registrador
     */
    static std::string createInstruction(const std::string &instruction, const int r1, const int r2, const int r3);

    /**
     * @brief Preserva o valor de um registrador antes de executar uma ação
     * @param action Ação a ser executada
     * @param rg Registrador a ser preservado
     */
    static void preserveRegister(const int rg, const std::function<void()> &action);

    /**
     * @brief Preserva o valor de um registrador antes de executar uma ação para funcionar como um decorator Python
     * @param rg Registrador a ser preservado
     * @param action Ação a ser executada
     * @return A função que preservou o registrador
     */
    static std::function<void()> preserveRegisterInStack(const int rg, const std::function<void()> &action);
};

#endif