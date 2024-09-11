#include "utils.hpp"
#include <sstream>

bool SymbolTable::addSymbol(const std::string &name, const std::string &type, std::string &errorMsg) {
    auto it = table.find(name);

    if (it != table.end()) {
        if (it->second == type) {
            errorMsg = "identifier '" + name + "' already declared";
        } else {
            errorMsg = "redefinition of identifier '" + name + "'";
        }
        return false;
    }

    table[name] = type;
    return true;
}

void SymbolTable::clearTable() {
    table.clear();
}
