#include <iostream>
#include "lexer.hpp"

int main() {
    MyLexer lexer(std::cin, std::cout); // Passa std::cin e std::cout como referências
    Token token;
    while ((token = lexer.nextToken()) != Token::ERROR) {
        switch (token) {
            case Token::STRING1:
                std::cout << "Token: STRING1, Texto: " << lexer.getTokenText() << std::endl;
                break;
            case Token::STRING2:
                std::cout << "Token: STRING2, Texto: " << lexer.getTokenText() << std::endl;
                break;
            case Token::STRING3:
                std::cout << "Token: STRING3, Texto: " << lexer.getTokenText() << std::endl;
                break;
            default:
                std::cout << "Token não reconhecido: " << static_cast<int>(token) << std::endl;
        }
    }
    return 0;
}
