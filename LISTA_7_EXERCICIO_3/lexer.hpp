#ifndef LEXER_HPP
#define LEXER_HPP

#include <FlexLexer.h>
#include <string>
#include <unordered_map>
#include "tokens.hpp"

/**
 * @brief Classe usada para fazer o léxico usando o Flex
 */
class MyLexer : public yyFlexLexer
{
private:
    Token current_token;
    std::string current_text;
    static const std::unordered_map<Token, std::string> tokenMap;

    /**
     * @brief Obtém a string do token
     * @return Retorna a string
     */
    std::string YYText() const
    {
        return std::string(yytext, yyleng);
    }

public:
    /**
     * @brief Construtor do Léxico
     * @param in Stream de entrada de dados
     * @param out Stream de saída de dados
     */
    MyLexer(std::istream &in = std::cin, std::ostream &out = std::cout)
        : yyFlexLexer(&in, &out), current_token(Token::END_OF_FILE) {}

    /**
     * @brief Obtém o próximo token
     * @return Retorna o token definido pelo enum
     */
    Token nextToken()
    {
        int token = yylex(); // Chama o método yylex da classe base
        current_token = static_cast<Token>(token);
        current_text = YYText(); // Obtém o texto atual do token
        return current_token;
    }

    /**
     * @brief Obtém o token atual
     * @return Retorna o token atual
     */
    Token getTokenValue() const
    {
        return current_token;
    }

    /**
     * @brief Obtém o texto do token atual
     * @return Retorna o texto
     */
    const std::string &getTokenText() const
    {
        return current_text;
    }

    /**
     * @brief Obtém a string associada ao token atual
     * @return Retorna a string associada ao token atual
     */
    std::string getTokenString() const
    {
        auto it = tokenMap.find(current_token);
        if (it != tokenMap.end())
        {
            return it->second;
        }
        else
        {
            return "UNKNOWN";
        }
    }

    /**
     * @brief Define o token atual
     * @param value Valor do Token atual
     */
    void setTokenValue(Token value)
    {
        current_token = value;
    }

    /**
     * @brief Define o texto do token atual
     * @param text Texto do token
     */
    void setTokenText(const std::string &text)
    {
        current_text = text;
    }
};

/**
 * @brief Mapeamento de tokens para strings
 */
const std::unordered_map<Token, std::string> MyLexer::tokenMap = {
        {Token::END_OF_FILE, "END_OF_FILE"},
        {Token::ACTION1, "(action 1)"},
        {Token::ACTION2, "(action 2)"},
        {Token::ACTION3, "(action 3)"},
        {Token::ERROR, "error"},
};

#endif
