#include "mips.hpp"
#include <iostream>

std::queue<std::string> MIPS::data;
std::queue<std::string> MIPS::text;
bool MIPS::arg_registers[ARGUMENT_REGISTER];
bool MIPS::temp_registers[TEMPORARY_REGISTER];
bool MIPS::save_registers[SAVE_REGISTER];
int MIPS::string_count = 0;
std::pair<int, int> MIPS::if_count = {0, 0};
std::pair<int, int> MIPS::while_count = {0, 0};
std::pair<int, int> MIPS::for_count = {0, 0};

void MIPS::initialize()
{
    // Inicializa todos os registradores argumentos como true
    for (int i = 0; i < ARGUMENT_REGISTER; ++i)
    {
        arg_registers[i] = true;
    }
    // Inicializa todos os registradores temporários como true
    for (int i = 0; i < TEMPORARY_REGISTER; ++i)
    {
        temp_registers[i] = true;
    }
    // Inicializa todos os registradores salvos como true
    for (int i = 0; i < SAVE_REGISTER; ++i)
    {
        save_registers[i] = true;
    }
}

int MIPS::getArgumentRegister()
{
    for (int i = 0; i < ARGUMENT_REGISTER; ++i)
    {
        if (arg_registers[i])
        {
            arg_registers[i] = false;
            return i + 1;
        }
    }
    return -1;
}

int MIPS::getTemporaryRegister()
{
    for (int i = 0; i < TEMPORARY_REGISTER; ++i)
    {
        if (temp_registers[i])
        {
            temp_registers[i] = false;
            return ARGUMENT_REGISTER + i + 1;
        }
    }
    return -1;
}

int MIPS::getSaveRegister()
{
    for (int i = 0; i < SAVE_REGISTER; ++i)
    {
        if (save_registers[i])
        {
            save_registers[i] = false;
            return ARGUMENT_REGISTER + TEMPORARY_REGISTER + i + 1;
        }
    }
    return -1;
}

void MIPS::freeArgumentRegister(const int index)
{
    arg_registers[index - 1] = true;
}

void MIPS::freeTemporaryRegister(const int index)
{
    temp_registers[index - (ARGUMENT_REGISTER + 1)] = true;
}

void MIPS::freeSaveRegister(const int index)
{
    save_registers[index - (ARGUMENT_REGISTER + TEMPORARY_REGISTER + 1)] = true;
}

std::string MIPS::getRegisterName(const int index)
{
    if (index == 0)
    {
        return "$zero";
    }
    else if (index < ARGUMENT_REGISTER)
    {
        return "$a" + std::to_string(index - 1);
    }
    else if (index < (ARGUMENT_REGISTER + TEMPORARY_REGISTER))
    {
        return "$t" + std::to_string(index - (ARGUMENT_REGISTER + 1));
    }
    else if (index < (ARGUMENT_REGISTER + TEMPORARY_REGISTER + SAVE_REGISTER))
    {
        return "$s" + std::to_string(index - (ARGUMENT_REGISTER + TEMPORARY_REGISTER + 1));
    }
    else
    {
        throw std::invalid_argument("Invalid register index");
    }
}

void MIPS::printCode()
{
    std::cout << ".data" << std::endl;
    while (!data.empty())
    {
        std::cout << "\t" << data.front() << std::endl;
        data.pop();
    }
    std::cout << std::endl;
    std::cout << ".text" << std::endl;
    while (!text.empty())
    {
        std::cout << "\t" << text.front() << std::endl;
        text.pop();
    }
}

void MIPS::createConstant(const std::string &identifier, const int constant)
{
    data.push(identifier + " = " + std::to_string(constant));
}

std::string MIPS::createString(const std::string &string)
{
    std::string label = "string_" + std::to_string(++string_count);
    data.push(label + ": .asciiz \"" + string + "\"");
    return label;
}

void MIPS::createGlobalVar(const std::string &identifier, const int value)
{
    data.push(identifier + ": .word " + std::to_string(value));
}

void MIPS::createExpression(const OperatorType op, const int r1, const int r2, const int rg_result)
{
}

void MIPS::createExpression(const OperatorType op, const int r1, const int rg_result)
{
}

void MIPS::createArrayAccess(const std::string &array_identifier, const int rg_index, const int rg_result)
{
    // Calcula o deslocamento (index * 4, assumindo elementos de 4 bytes (word size))
    int reg_temp = getTemporaryRegister();
    text.push("sll " + getRegisterName(reg_temp) + ", " + getRegisterName(rg_index) + ", 2");
    text.push("la " + getRegisterName(rg_result) + ", " + array_identifier);
    text.push("add " + getRegisterName(rg_result) + ", " + getRegisterName(rg_result) + ", " + getRegisterName(reg_temp));
    text.push("lw " + getRegisterName(rg_result) + ", 0(" + getRegisterName(rg_result) + ")");
    freeTemporaryRegister(reg_temp);
}

void MIPS::initializeConstant(const int rg_dst, const int value)
{
    text.push(createInstruction("addi", rg_dst, 0, value));
}

std::string MIPS::startWhile()
{
    std::string label = "while_" + std::to_string(++while_count.first);
    createLabel(label);
    return label;
}

std::string MIPS::getEndWhile()
{
    return "end_while_" + std::to_string(while_count.first);
}

void MIPS::endWhile()
{
    createLabel("end_while_" + std::to_string(++while_count.second));
}

void MIPS::startIf()
{
    createLabel("if_" + std::to_string(++if_count.first));
}

std::string MIPS::getElse()
{
    return "else_" + std::to_string(if_count.first);
}

void MIPS::startElse()
{
    createLabel("else_" + std::to_string(if_count.first));
}

std::string MIPS::getEndIf()
{
    return "end_if_" + std::to_string(if_count.first);
}

void MIPS::endIf()
{
    createLabel("end_if_" + std::to_string(++if_count.second));
}

std::string MIPS::startFor()
{
    std::string label = "for_" + std::to_string(++for_count.first);
    createLabel(label);
    return label;
}

std::string MIPS::getEndFor()
{
    return "end_for_" + std::to_string(for_count.first);
}

void MIPS::endFor()
{
    createLabel("end_for_" + std::to_string(++for_count.second));
}

void MIPS::startTernary()
{
    createLabel("ternary_" + std::to_string(++ternary_count.first));
}

std::string MIPS::getElseTernary()
{
    return "else_ternary_" + std::to_string(ternary_count.first);
}

void MIPS::startElseTernary()
{
    createLabel("else_ternary_" + std::to_string(ternary_count.first));
}

std::string MIPS::getEndTernary()
{
    return "end_ternary_" + std::to_string(ternary_count.first);
}

void MIPS::endTernary()
{
    createLabel("end_ternary_" + std::to_string(++ternary_count.second));
}

void MIPS::createLabel(const std::string &label)
{
    text.push(label + ":");
}

void MIPS::jumpTo(const std::string &label)
{
    text.push("j " + label);
}

void MIPS::moveTo(const int r_src, const int r_dst)
{
    text.push("move " + getRegisterName(r_dst) + ", " + getRegisterName(r_src));
}

void MIPS::branchEqualZero(const int rg, const std::string &label)
{
    text.push("beq $zero, " + getRegisterName(rg) + ", " + label);
}

void MIPS::branchNotEqualZero(const int rg, const std::string &label)
{
    text.push("bne $zero, " + getRegisterName(rg) + ", " + label);
}

void MIPS::getAddress(const int rg, const std::string &label)
{
    text.push("la " + getRegisterName(rg) + ", " + label);
}

void MIPS::callFunction(const std::string &function_name)
{
    text.push("jal " + function_name);
}

void MIPS::callPrintf(const std::string &string)
{
    int reg = getTemporaryRegister();
    text.push("move " + getRegisterName(reg) + ", $a0");
    text.push("li $v0, 4");
    text.push("la $a0, " + string);
    text.push("syscall");
    text.push("move $a0, " + getRegisterName(reg));
    freeTemporaryRegister(reg);
}

void MIPS::callPrintf(const int value)
{
    int reg = getTemporaryRegister();
    text.push("move " + getRegisterName(reg) + ", $a0");
    text.push("li $v0, 1");
    text.push("move $a0, " + getRegisterName(value));
    text.push("syscall");
    text.push("move $a0, " + getRegisterName(reg));
    freeTemporaryRegister(reg);
}

void MIPS::callPrintfAsString(const int rg)
{
    int reg = getTemporaryRegister();
    text.push("move " + getRegisterName(reg) + ", $a0");
    text.push("li $v0, 4");
    text.push("move $a0, " + getRegisterName(rg));
    text.push("syscall");
    text.push("move $a0, " + getRegisterName(reg));
    freeTemporaryRegister(reg);
}

void MIPS::callPrintfAsChar(const int rg)
{
    int reg = getTemporaryRegister();
    text.push("move " + getRegisterName(reg) + ", $a0");
    text.push("li $v0, 11");
    text.push("move $a0, " + getRegisterName(rg));
    text.push("syscall");
    text.push("move $a0, " + getRegisterName(reg));
    freeTemporaryRegister(reg);
}

void MIPS::callScanf(const std::string &identifier)
{
    text.push("li $v0, 5");
    text.push("syscall");
    text.push("sw $v0, " + identifier);
}

void MIPS::callReturn(const int rg)
{
    if (rg != -1)
    {
        text.push("move $v0, " + getRegisterName(rg));
    }
    text.push("jr $ra");
}

void MIPS::callExit(const int rg)
{
    if (rg != -1)
    {
        text.push("move $a0, " + getRegisterName(rg));
    }
    text.push("li $v0, 10");
    text.push("syscall");
}

std::string MIPS::createInstruction(const std::string &instruction, const int r1, const int r2, const int r3)
{
    return instruction + "  " + getRegisterName(r1) + ", " + getRegisterName(r2) + ", " + getRegisterName(r3);
}
