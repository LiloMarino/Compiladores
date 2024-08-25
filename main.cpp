#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include "automato_finito.hpp"
#include "lexico.hpp"
#include "sintatico.hpp"

using namespace std;

void createAutomato(AutomatoFinito &af)
{
    af.addRegularExpression("[a-z][a-z0-9]*", "id");
    af.addRegularExpression("+", "+");
    af.addRegularExpression("*", "*");
    af.addRegularExpression("(", "(");
    af.addRegularExpression(")", ")");
    af.addRegularExpression("$", "$");
    af.toAFD();
    af.minimizeAFD();
}

void createParseTable(AnalisadorSintatico &as)
{
    as.addProduction("E", "id", "T E'");
    as.addProduction("T", "id", "F T'");
    as.addProduction("F", "id", "id");
    as.addProduction("S", "id", "E $");
    as.addProduction("E'", "+", "+ T E'");
    as.addProduction("T'", "+", "");
    as.addProduction("T'", "*", "* F T'");
    as.addProduction("E", "(", "T E'");
    as.addProduction("T", "(", "F T'");
    as.addProduction("F", "(", "( E )");
    as.addProduction("S", "(", "E $");
    as.addProduction("E'", ")", "");
    as.addProduction("T'", ")", "");
    as.addProduction("E'", "$", "");
    as.addProduction("T'", "$", "");
}

int main()
{
    AutomatoFinito af;
    createAutomato(af);
    AnalisadorLexico lexic(af);
    lexic.addIgnoreSymbol(' ');
    AnalisadorSintatico sintatic("S");
    createParseTable(sintatic);
    string input;
    while (getline(cin, input))
    {
        try
        {
            list<LexicalGroup> tokens = lexic.reconhecer(input);
            sintatic.analisar(tokens);
            std::cout << "CADEIA ACEITA";
        }
        catch(const AnalisadorSintatico::SintaticError& e)
        {
            std::cerr << e.what() << std::endl;
        }
        catch(const AnalisadorLexico::LexicalError& e)
        {
            std::cerr << e.what() << std::endl;
        }
    }
    return 0;
}