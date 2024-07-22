#include <iostream>
#include "lexer.hpp"

int main() {
    MyLexer lexer(std::cin, std::cout);
    Token token;
    while ((token = lexer.nextToken()) != Token::END_OF_FILE) {
        switch (token) {
            case Token::VALIDO:
                std::cout << lexer.getTokenText() << std::endl;
                break;
            case Token::INVALIDO:
                std::cout << "ERRO" << std::endl;
                break;
            default:
                std::cout << "???" << std::endl;
                break;
        }
    }
    return 0;
}
