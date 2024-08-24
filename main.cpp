#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include "automato_finito.hpp"
#include "lexico.hpp"

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

int main()
{
    AutomatoFinito af;
    createAutomato(af);
    AnalisadorLexico al(af);
    al.addIgnoreSymbol(' ');
    string input;
    while (getline(cin, input))
    {
        list<recon> aux = al.reconhecer(input);
        for (recon &token : aux)
        {
            cout << token.cadeia << " " << token.token << endl;
        }
    }
    return 0;
}