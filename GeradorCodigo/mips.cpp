#include "mips.hpp"
#include <iostream>

std::queue<std::string> MIPS::data;
std::queue<std::string> MIPS::text;
bool MIPS::arg_registers[ARGUMENT_REGISTER];
bool MIPS::temp_registers[TEMPORARY_REGISTER];
bool MIPS::save_registers[SAVE_REGISTER];
int MIPS::string_count = 0;
std::pair<int, std::stack<int>> MIPS::ternary_data = {0, {}};
std::pair<int, std::stack<int>> MIPS::if_data = {0, {}};
std::pair<int, std::stack<int>> MIPS::while_data = {0, {}};
std::pair<int, std::stack<int>> MIPS::for_data = {0, {}};

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
    if (index > 0 && index <= ARGUMENT_REGISTER)
    {
        arg_registers[index - 1] = true;
    }
}

void MIPS::freeTemporaryRegister(const int index)
{
    if (index > ARGUMENT_REGISTER && index <= (ARGUMENT_REGISTER + TEMPORARY_REGISTER))
    {
        temp_registers[index - (ARGUMENT_REGISTER + 1)] = true;
    }
}

void MIPS::freeSaveRegister(const int index)
{
    if (index > (ARGUMENT_REGISTER + TEMPORARY_REGISTER) && index <= (ARGUMENT_REGISTER + TEMPORARY_REGISTER + SAVE_REGISTER))
    {
        save_registers[index - (ARGUMENT_REGISTER + TEMPORARY_REGISTER + 1)] = true;
    }
}

void MIPS::freeAllRegisters()
{
    for (int i = 0; i < ARGUMENT_REGISTER; ++i)
    {
        arg_registers[i] = true;
    }
    for (int i = 0; i < TEMPORARY_REGISTER; ++i)
    {
        temp_registers[i] = true;
    }
    for (int i = 0; i < SAVE_REGISTER; ++i)
    {
        save_registers[i] = true;
    }
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
    else if (index == RETURN_REGISTER)
    {
        return "$v0";
    }
    else if (index == RETURN_ADDRESS)
    {
        return "$ra";
    }
    else
    {
        throw std::invalid_argument("Invalid register index");
    }
}

int MIPS::getRegisterIndex(const std::string &name)
{
    if (name == "$zero")
    {
        return 0;
    }
    else if (name[1] == 'a')
    {
        return 1 + std::stoi(name.substr(2));
    }
    else if (name[1] == 't')
    {
        return ARGUMENT_REGISTER + 1 + std::stoi(name.substr(2));
    }
    else if (name[1] == 's')
    {
        return ARGUMENT_REGISTER + TEMPORARY_REGISTER + 1 + std::stoi(name.substr(2));
    }
    else if (name == "$v0")
    {
        return RETURN_REGISTER;
    }
    else if (name == "$ra")
    {
        return RETURN_ADDRESS;
    }
    else
    {
        throw std::invalid_argument("Invalid register name");
    }
}

bool MIPS::isRegisterInUse(const int index)
{
    if (index == 0)
    {
        return true;
    }
    else if (index < ARGUMENT_REGISTER)
    {
        return !arg_registers[index - 1];
    }
    else if (index < (ARGUMENT_REGISTER + TEMPORARY_REGISTER))
    {
        return !temp_registers[index - ARGUMENT_REGISTER - 1];
    }
    else if (index < (ARGUMENT_REGISTER + TEMPORARY_REGISTER + SAVE_REGISTER))
    {
        return !save_registers[index - ARGUMENT_REGISTER - TEMPORARY_REGISTER - 1];
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

void MIPS::createGlobalVar(const std::string &identifier, const Type *type)
{
    switch (type->getType())
    {
    case TypeEnum::INT:
        data.push(identifier + ": .word 0");
        break;
    case TypeEnum::CHAR:
        data.push(identifier + ": .byte 0");
        break;
    case TypeEnum::VOID_POINTER:
        data.push(identifier + ": .word 0");
        break;
    case TypeEnum::ARRAY_INT:
    {
        int arraySize = 4;
        for (auto &dim : type->getDimensions())
        {
            arraySize *= dim;
        }
        data.push(identifier + ": .space " + std::to_string(arraySize));
    }
    break;
    case TypeEnum::ARRAY_CHAR:
    {
        int arraySize = 1;
        for (auto &dim : type->getDimensions())
        {
            arraySize *= dim;
        }
        data.push(identifier + ": .space " + std::to_string(arraySize));
    }
    break;
    default:
        throw std::invalid_argument("Unsupported type for global variable: " + identifier);
    }
}

void MIPS::createExpression(const OperatorType op, const int r1, const int r2, const int rg_result)
{
    switch (op)
    {
    case OperatorType::PLUS:
        text.push("add " + getRegisterName(rg_result) + ", " + getRegisterName(r1) + ", " + getRegisterName(r2));
        break;
    case OperatorType::MINUS:
        text.push("sub " + getRegisterName(rg_result) + ", " + getRegisterName(r1) + ", " + getRegisterName(r2));
        break;
    case OperatorType::MULTIPLY:
        text.push("mul " + getRegisterName(rg_result) + ", " + getRegisterName(r1) + ", " + getRegisterName(r2));
        break;
    case OperatorType::DIVIDE:
        text.push("div " + getRegisterName(r1) + ", " + getRegisterName(r2));
        text.push("mflo " + getRegisterName(rg_result));
        break;
    case OperatorType::REMAINDER:
        text.push("div " + getRegisterName(r1) + ", " + getRegisterName(r2));
        text.push("mfhi " + getRegisterName(rg_result));
        break;
    case OperatorType::BITWISE_AND:
        text.push("and " + getRegisterName(rg_result) + ", " + getRegisterName(r1) + ", " + getRegisterName(r2));
        break;
    case OperatorType::BITWISE_OR:
        text.push("or " + getRegisterName(rg_result) + ", " + getRegisterName(r1) + ", " + getRegisterName(r2));
        break;
    case OperatorType::BITWISE_XOR:
        text.push("xor " + getRegisterName(rg_result) + ", " + getRegisterName(r1) + ", " + getRegisterName(r2));
        break;
    case OperatorType::LOGICAL_AND:
        text.push("sne " + getRegisterName(r1) + ", " + getRegisterName(r1) + ", $zero");
        text.push("sne " + getRegisterName(r2) + ", " + getRegisterName(r2) + ", $zero");
        text.push("and " + getRegisterName(rg_result) + ", " + getRegisterName(r1) + ", " + getRegisterName(r2));
        break;
    case OperatorType::LOGICAL_OR:
        text.push("sne " + getRegisterName(r1) + ", " + getRegisterName(r1) + ", $zero");
        text.push("sne " + getRegisterName(r2) + ", " + getRegisterName(r2) + ", $zero");
        text.push("or " + getRegisterName(rg_result) + ", " + getRegisterName(r1) + ", " + getRegisterName(r2));
        break;
    case OperatorType::EQUAL:
        text.push("sub " + getRegisterName(rg_result) + ", " + getRegisterName(r1) + ", " + getRegisterName(r2));
        text.push("seq " + getRegisterName(rg_result) + ", " + getRegisterName(rg_result) + ", $zero");
        break;
    case OperatorType::NOT_EQUAL:
        text.push("sub " + getRegisterName(rg_result) + ", " + getRegisterName(r1) + ", " + getRegisterName(r2));
        text.push("sne " + getRegisterName(rg_result) + ", " + getRegisterName(rg_result) + ", $zero");
        break;
    case OperatorType::LESS_THAN:
        text.push("slt " + getRegisterName(rg_result) + ", " + getRegisterName(r1) + ", " + getRegisterName(r2));
        break;
    case OperatorType::GREATER_THAN:
        text.push("sgt " + getRegisterName(rg_result) + ", " + getRegisterName(r1) + ", " + getRegisterName(r2));
        break;
    case OperatorType::LESS_EQUAL:
        text.push("sle " + getRegisterName(rg_result) + ", " + getRegisterName(r1) + ", " + getRegisterName(r2));
        break;
    case OperatorType::GREATER_EQUAL:
        text.push("sge " + getRegisterName(rg_result) + ", " + getRegisterName(r1) + ", " + getRegisterName(r2));
        break;
    case OperatorType::R_SHIFT:
        text.push("srl " + getRegisterName(rg_result) + ", " + getRegisterName(r1) + ", " + getRegisterName(r2));
        break;
    case OperatorType::L_SHIFT:
        text.push("sll " + getRegisterName(rg_result) + ", " + getRegisterName(r1) + ", " + getRegisterName(r2));
        break;
    default:
        throw std::runtime_error("Invalid operator");
        break;
    }
}

void MIPS::createExpression(const OperatorType op, const int r1, const int rg_result)
{
    switch (op)
    {
    case OperatorType::PLUS:
        text.push("add " + getRegisterName(rg_result) + ", " + getRegisterName(r1) + ", $zero");
        break;
    case OperatorType::MINUS:
        text.push("sub " + getRegisterName(rg_result) + ", $zero, " + getRegisterName(r1));
        break;
    case OperatorType::MULTIPLY:
        text.push("lw " + getRegisterName(rg_result) + ", 0(" + getRegisterName(r1) + ")");
        break;
    case OperatorType::INC:
        text.push("addi " + getRegisterName(rg_result) + ", " + getRegisterName(r1) + ", 1");
        break;
    case OperatorType::DEC:
        text.push("addi " + getRegisterName(rg_result) + ", " + getRegisterName(r1) + ", -1");
        break;
    case OperatorType::BITWISE_NOT:
        text.push("nor " + getRegisterName(rg_result) + ", " + getRegisterName(r1) + ", $zero");
        break;
    case OperatorType::NOT:
        text.push("seq " + getRegisterName(rg_result) + ", " + getRegisterName(r1) + ", $zero");
        break;
    default:
        throw std::runtime_error("Invalid operator");
        break;
    }
}

void MIPS::createArrayAccess(const std::string &array_identifier, const int rg_index, const int rg_dst)
{
    // Calcula o deslocamento (index * 4, assumindo elementos de 4 bytes (word size))
    int reg_temp = getTemporaryRegister();
    text.push("sll " + getRegisterName(reg_temp) + ", " + getRegisterName(rg_index) + ", 2");
    text.push("la " + getRegisterName(rg_dst) + ", " + array_identifier);
    text.push("add " + getRegisterName(rg_dst) + ", " + getRegisterName(rg_dst) + ", " + getRegisterName(reg_temp));
    text.push("lw " + getRegisterName(rg_dst) + ", 0(" + getRegisterName(rg_dst) + ")");
    freeTemporaryRegister(reg_temp);
}

void MIPS::createArraySave(const std::string &array_identifier, const int rg_index, const int rg_src)
{
    int reg_temp = getTemporaryRegister();
    int reg_address = getTemporaryRegister();
    text.push("sll " + getRegisterName(reg_temp) + ", " + getRegisterName(rg_index) + ", 2");
    text.push("la " + getRegisterName(reg_address) + ", " + array_identifier);
    text.push("add " + getRegisterName(reg_address) + ", " + getRegisterName(reg_address) + ", " + getRegisterName(reg_temp));
    text.push("sw " + getRegisterName(rg_src) + ", 0(" + getRegisterName(reg_address) + ")");
    freeTemporaryRegister(reg_temp);
    freeTemporaryRegister(reg_address);
}

void MIPS::initializeConstant(const int rg_dst, const int value)
{
    text.push("addi " + getRegisterName(rg_dst) + ", " + getRegisterName(0) + ", " + std::to_string(value));
}

std::string MIPS::startWhile()
{
    int new_index = ++while_data.first;
    while_data.second.push(new_index);
    std::string label = "while_" + std::to_string(new_index);
    createLabel(label);
    return label;
}

std::string MIPS::getEndWhile()
{
    if (while_data.second.empty())
        throw std::runtime_error("No active 'while' block to end.");
    return "end_while_" + std::to_string(while_data.second.top());
}

void MIPS::endWhile()
{
    if (while_data.second.empty())
        throw std::runtime_error("No active 'while' block to end.");
    createLabel("end_while_" + std::to_string(while_data.second.top()));
    while_data.second.pop();
}

void MIPS::startIf()
{
    int new_index = ++if_data.first;
    if_data.second.push(new_index);
    createLabel("if_" + std::to_string(new_index));
}

std::string MIPS::getElse()
{
    if (if_data.second.empty())
        throw std::runtime_error("No active 'if' block to get 'else'.");
    return "else_" + std::to_string(if_data.second.top());
}

void MIPS::startElse()
{
    if (if_data.second.empty())
        throw std::runtime_error("No active 'if' block to start 'else'.");
    createLabel("else_" + std::to_string(if_data.second.top()));
}

std::string MIPS::getEndIf()
{
    if (if_data.second.empty())
        throw std::runtime_error("No active 'if' block to end.");
    return "end_if_" + std::to_string(if_data.second.top());
}

void MIPS::endIf()
{
    if (if_data.second.empty())
        throw std::runtime_error("No active 'if' block to end.");
    createLabel("end_if_" + std::to_string(if_data.second.top()));
    if_data.second.pop();
}

std::string MIPS::startFor()
{
    int new_index = ++for_data.first;
    for_data.second.push(new_index);
    std::string label = "for_" + std::to_string(new_index);
    createLabel(label);
    return label;
}

std::string MIPS::getEndFor()
{
    if (for_data.second.empty())
        throw std::runtime_error("No active 'for' block to end.");
    return "end_for_" + std::to_string(for_data.second.top());
}

void MIPS::endFor()
{
    if (for_data.second.empty())
        throw std::runtime_error("No active 'for' block to end.");
    createLabel("end_for_" + std::to_string(for_data.second.top()));
    for_data.second.pop();
}

void MIPS::startTernary()
{
    int new_index = ++ternary_data.first;
    ternary_data.second.push(new_index);
    createLabel("ternary_" + std::to_string(new_index));
}

std::string MIPS::getElseTernary()
{
    if (ternary_data.second.empty())
        throw std::runtime_error("No active 'ternary' block to get 'else'.");
    return "else_ternary_" + std::to_string(ternary_data.second.top());
}

void MIPS::startElseTernary()
{
    if (ternary_data.second.empty())
        throw std::runtime_error("No active 'ternary' block to start 'else'.");
    createLabel("else_ternary_" + std::to_string(ternary_data.second.top()));
}

std::string MIPS::getEndTernary()
{
    if (ternary_data.second.empty())
        throw std::runtime_error("No active 'ternary' block to end.");
    return "end_ternary_" + std::to_string(ternary_data.second.top());
}

void MIPS::endTernary()
{
    if (ternary_data.second.empty())
        throw std::runtime_error("No active 'ternary' block to end.");
    createLabel("end_ternary_" + std::to_string(ternary_data.second.top()));
    ternary_data.second.pop();
}

void MIPS::createLabel(const std::string &label)
{
    text.push(label + ":");
}

void MIPS::jumpTo(const std::string &label)
{
    text.push("j " + label);
}

void MIPS::moveTo(const int rg_src, const int rg_dst)
{
    text.push("move " + getRegisterName(rg_dst) + ", " + getRegisterName(rg_src));
}

void MIPS::loadWord(const std::string &identifier, const int rg_dst)
{
    text.push("lw " + getRegisterName(rg_dst) + ", " + identifier);
}

void MIPS::saveWord(const std::string &identifier, const int rg_src)
{
    text.push("sw " + getRegisterName(rg_src) + ", " + identifier);
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
    auto function = [&]()
    {
        text.push("jal " + function_name);
    };

    // Salva o contexo por meio de uma espécie de sistema de 'decorator (do Python)'
    std::function<void()> currentAction = function; // Variável que mantém a cadeia de lambdas

    for (int i = 0; i < TEMPORARY_REGISTER; ++i)
    {
        if (!temp_registers[i])
        {
            int rg = getRegisterIndex("$t" + std::to_string(i));
            currentAction = preserveRegisterInStack(rg, currentAction); // Encadeia a lambda
        }
    }

    for (int i = 0; i < SAVE_REGISTER; ++i)
    {
        if (!save_registers[i])
        {
            int rg = getRegisterIndex("$s" + std::to_string(i));
            currentAction = preserveRegisterInStack(rg, currentAction); // Encadeia a lambda
        }
    }

    // Salva o endereço de retorno
    currentAction = preserveRegisterInStack(getRegisterIndex("$ra"), currentAction); // Encadeia a lambda

    // Após percorrer todos os registradores, executa a ação final
    currentAction();
}

void MIPS::callPrintf(const std::string &string)
{
    int a0 = getRegisterIndex("$a0");
    auto print = [&]()
    {
        text.push("li $v0, 4");
        text.push("la $a0, " + createString(string));
        text.push("syscall");
    };
    if (isRegisterInUse(a0))
    {
        preserveRegister(a0, print);
    }
    else
    {
        print();
    }
}

void MIPS::callPrintf(const int value)
{
    int a0 = getRegisterIndex("$a0");
    auto print = [&]()
    {
        text.push("li $v0, 1");
        text.push("move $a0, " + getRegisterName(value));
        text.push("syscall");
    };
    if (isRegisterInUse(a0))
    {
        preserveRegister(a0, print);
    }
    else
    {
        print();
    }
}

void MIPS::callPrintfAsString(const int rg)
{
    int a0 = getRegisterIndex("$a0");
    auto print = [&]()
    {
        text.push("li $v0, 4");
        text.push("move $a0, " + getRegisterName(rg));
        text.push("syscall");
    };
    if (isRegisterInUse(a0))
    {
        preserveRegister(a0, print);
    }
    else
    {
        print();
    }
}

void MIPS::callPrintfAsChar(const int rg)
{
    int a0 = getRegisterIndex("$a0");
    auto print = [&]()
    {
        text.push("li $v0, 11");
        text.push("move $a0, " + getRegisterName(rg));
        text.push("syscall");
    };
    if (isRegisterInUse(a0))
    {
        preserveRegister(a0, print);
    }
    else
    {
        print();
    }
}

void MIPS::callScanf(const int rg)
{
    text.push("li $v0, 5");
    text.push("syscall");
    text.push("move " + getRegisterName(rg) + ", $v0");
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

void MIPS::addInstruction(const std::string &instruction)
{
    text.push(instruction);
}

void MIPS::preserveRegister(const int rg, const std::function<void()> &action)
{
    std::string tempReg = getRegisterName(getTemporaryRegister());
    text.push("move " + tempReg + ", " + getRegisterName(rg)); // Salva o valor do registrador
    action();
    text.push("move " + getRegisterName(rg) + ", " + tempReg); // Restaura o valor do registrador
    freeTemporaryRegister(getRegisterIndex(tempReg));
}

std::function<void()> MIPS::preserveRegisterInStack(const int rg, const std::function<void()> &action)
{
    // Retorna uma função lambda que realiza a ação e restaura o registrador
    return [=]()
    {
        // Aloca o registrador na pilha
        text.push("addi $sp, $sp, -4");
        text.push("sw " + getRegisterName(rg) + ", 0($sp)");

        action(); // Chama a ação passada para a função (como um decorator)

        // Restaura o registrador da pilha
        text.push("lw " + getRegisterName(rg) + ", 0($sp)");
        text.push("addi $sp, $sp, 4");
    };
}
