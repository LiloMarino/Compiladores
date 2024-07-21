#include <iostream>
#include "lexer.hpp"

int main() {
    int token;
    while ((token = yylex()) != 0) {
        switch (token) {
            case IDENTIFIER:
                std::cout << "Identifier: " << yytext << std::endl;
                break;
            case NUMBER:
                std::cout << "Number: " << yytext << std::endl;
                break;
            default:
                std::cout << "Other: " << static_cast<char>(token) << std::endl;
        }
    }
    return 0;
}
