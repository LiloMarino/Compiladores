#ifndef UTILS_HPP
#define UTILS_HPP

#include <string>
#include <unordered_map>

struct Symbol {
    std::string type;
    std::string name;
};

class SymbolTable {
public:
    bool addSymbol(const std::string &name, const std::string &type, std::string &errorMsg);
    void clearTable();

private:
    std::unordered_map<std::string, std::string> table;
};

#endif
