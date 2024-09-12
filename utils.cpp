#include "utils.hpp"
#include <vector>
#include <sstream>
#include <iostream>

extern int yylineno;
extern bool error;
SymbolTable symbolTable;

void SymbolTable::addSymbol(const std::string &type, const std::string &declaration)
{
    // Separa as , e obtém os identificadores separados
    std::vector<std::string> identificadores;
    std::istringstream stream(declaration);
    std::string identificador;
    while (std::getline(stream, identificador, ','))
    {
        identificadores.push_back(identificador);
    }

    // Para cada identificador trata ele e adiciona na tabela de símbolos
    for (const auto &id : identificadores)
    {
        auto it = table.find(id);
        if (it != table.end())
        {
            if (it->second == type)
            {
                showMessage("identifier '" + id + "' already declared");
                error = true;
            }
            else
            {
                showMessage("redefinition of identifier '" + id + "'");
                error = true;
            }
            return;
        }
        table[id] = type;
    }
}

void SymbolTable::clearTable()
{
    table.clear();
}

void showMessage(const std::string &msg)
{
    std::cout << yylineno << ": " << msg << std::endl;
}
