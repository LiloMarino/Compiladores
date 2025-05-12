#include <iostream>
#include "lexer.hpp"
#include "tokens.hpp"

int main()
{
    MyLexer lexer(std::cin, std::cout);
    Token token;
    while ((token = lexer.nextToken()) != Token::END_OF_FILE)
    {
        std::cout << lexer.getTokenText() << " " << lexer.getTokenString() << std::endl;
    }
    return 0;
}
