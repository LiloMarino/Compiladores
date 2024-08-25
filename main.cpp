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
    as.addProduction("E", "id", "TE'");
    as.addProduction("T", "id", "FT'");
    as.addProduction("F", "id", "id");
    as.addProduction("S", "id", "E$");
    as.addProduction("E'", "+", "+TE'");
    as.addProduction("T'", "+", "");
    as.addProduction("T'", "*", "*FT'");
    as.addProduction("E", "(", "TE'");
    as.addProduction("T", "(", "FT'");
    as.addProduction("F", "(", "(E)");
    as.addProduction("S", "(", "E$");
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
    string input;
    while (getline(cin, input))
    {
        list<LexicalGroup> aux = lexic.reconhecer(input);
        for (LexicalGroup &token : aux)
        {
            cout << token.cadeia << " " << token.token << endl;
        }
    }
    AnalisadorSintatico sintatic('S');
    createParseTable(sintatic);
    return 0;
}