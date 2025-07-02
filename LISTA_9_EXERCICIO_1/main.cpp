#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include "automato_finito.hpp"
#include "lexico.hpp"

using namespace std;

// Exceção Personalizada
class SintaticError : public std::exception
{
private:
    std::string mensagem;

public:
    explicit SintaticError(const std::string &msg) : mensagem(msg) {}

    virtual const char *what() const noexcept override
    {
        return mensagem.c_str();
    }
};

// Variáveis Globais
int token_id;
list<recon>::iterator it;
list<recon>::iterator it_begin;
TokenManager *manager;

// Funções Core
void advance()
{
    ++it;
    token_id = manager->getTokenId((*it).token);
}

void error(const std::vector<int> expected)
{
    std::string erro("ERRO SINTATICO EM: ");
    if ((*it).token == "num")
    {
        erro += "num";
    }
    else
    {
        erro += (*it).cadeia;
    }
    erro += " ESPERADO: ";
    for (const auto token : expected)
    {
        if (token == *(expected.begin()))
        {
            erro += manager->getToken(token);
        }
        else
        {
            erro += ", " + manager->getToken(token);
        }
    }
    throw SintaticError(erro);
}

void eat(int t)
{
    const std::vector<int> expect = {t};
    if (token_id == t)
        advance();
    else
        error(expect);
}

// Não terminais
void S();
void L();
void E();

void S()
{
    const std::vector<int> expect = {1, 4, 6};
    switch (token_id)
    {
    case 1:
        eat(1);
        E();
        eat(2);
        S();
        eat(3);
        S();
        break;
    case 4:
        eat(4);
        S();
        L();
        break;
    case 6:
        eat(6);
        E();
        break;
    default:
        error(expect);
    }
}

void L()
{
    const std::vector<int> expect = {5, 7};
    switch (token_id)
    {
    case 5:
        eat(5);
        break;
    case 7:
        eat(7);
        S();
        L();
        break;
    default:
        error(expect);
    }
}

void E()
{
    eat(8);
    eat(9);
    eat(8);
}

// Criação do Autômato
void createAutomato(AutomatoFinito &af)
{
    af.addRegularExpression("if", "if");           // 1
    af.addRegularExpression("then", "then");       // 2
    af.addRegularExpression("else", "else");       // 3
    af.addRegularExpression("begin", "begin");     // 4
    af.addRegularExpression("end", "end");         // 5
    af.addRegularExpression("print", "print");     // 6
    af.addRegularExpression(";", ";");             // 7
    af.addRegularExpression("[+-]?[0-9]+", "num"); // 8
    af.addRegularExpression("=", "=");             // 9
    af.toAFD();
}

int main()
{
    AutomatoFinito af;
    createAutomato(af);
    AnalisadorLexico al(af);
    al.addIgnoreSymbol(' ');
    string input;
    manager = &af.tokens;
    bool first_line = true;
    while (getline(cin, input))
    {
        list<recon> aux = al.reconhecer(input);
        it = aux.begin();
        it_begin = aux.begin();
        token_id = manager->getTokenId((*it).token);
        try
        {
            S();
            if (!first_line)
            {
                std::cout << std::endl;
            }
            std::cout << "CADEIA ACEITA";
            first_line = false;
        }
        catch (const SintaticError &e)
        {
            if (!first_line)
            {
                std::cout << std::endl;
            }
            std::cout << e.what();
            first_line = false;
        }
        catch (const std::exception &e)
        {
            if (!first_line)
            {
                std::cout << std::endl;
            }
            std::cout << "ERRO SINTATICO: CADEIA INCOMPLETA";
            first_line = false;
        }
    }
    return 0;
}
