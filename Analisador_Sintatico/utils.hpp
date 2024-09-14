#ifndef UTILS_HPP
#define UTILS_HPP

#include <string>
#include <unordered_map>

class SymbolTable {
public:
    void addSymbol(const std::string &type, const std::string &declaration);
    void clearTable();

private:
    std::unordered_map<std::string, std::string> table;
};

/**
 * @brief Imprime a mensagem no formato padrão
 * @param msg Mensagem
 */
void showMessage(const std::string &msg);

extern SymbolTable symbolTable;

#endif
