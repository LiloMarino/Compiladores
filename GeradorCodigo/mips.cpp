#include "mips.hpp"

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

void MIPS::freeTemporaryRegister(int index)
{
    temp_registers[index] = true;
}

void MIPS::freeSaveRegister(int index)
{
    save_registers[index] = true;
}

std::string MIPS::getRegisterName(int index)
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