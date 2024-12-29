#include "mips.hpp"
#include <iostream>

MIPS::MIPS()
{
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

int MIPS::getTemporaryRegister()
{
    for (int i = 0; i < TEMPORARY_REGISTER; i++)
    {
        if (temp_registers[i])
        {
            temp_registers[i] = false;
            return i;
        }
    }
    return -1;
}

int MIPS::getSaveRegister()
{
    for (int i = 0; i < SAVE_REGISTER; i++)
    {
        if (save_registers[i])
        {
            save_registers[i] = false;
            return TEMPORARY_REGISTER + i;
        }
    }
    return -1;
}

void MIPS::freeTemporaryRegister(const int index)
{
    temp_registers[index] = true;
}

void MIPS::freeSaveRegister(const int index)
{
    save_registers[index] = true;
}

std::string MIPS::getRegisterName(const int index)
{
    if (index < TEMPORARY_REGISTER)
    {
        return "t" + std::to_string(index);
    }
    else
    {
        return "s" + std::to_string(index - TEMPORARY_REGISTER);
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
