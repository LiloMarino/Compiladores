#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include "automato_finito.hpp"
#include "lexico.hpp"

using namespace std;

void createAutomato(AutomatoFinito &au)
{
    au.addTransitions(1, 2, "a-z");
    au.addTransitions(1, 3, "+");
    au.addTransitions(1, 4, "-");
    au.addTransitions(1, 5, "0-9");
    au.addTransitions(2, 2, "a-z");
    au.addTransitions(2, 2, "0-9");
    au.addTransitions(5, 5, "0-9");
    au.addTransitions(5, 6, "e");
    au.addTransitions(6, 8, "0-9");
    au.addTransitions(6, 7, "+,-");
    au.addTransitions(7, 8, "0-9");
    au.addTransitions(8, 8, "0-9");
}

int main()
{
    AutomatoFinito afd(8);
    createAutomato(afd);
    afd.setFinalState("ID", 2);
    afd.setFinalState("PLUS", 3);
    afd.setFinalState("MINUS", 4);
    afd.setFinalState("NUMBER", 5);
    afd.setFinalState("REAL", 8);
    afd.printVisualizacaoDOT("afd-o.dot");
    AutomatoFinito af;
    af.addRegularExpression("[a-z][a-z0-9]*", "ID");
    af.addRegularExpression("+", "PLUS");
    af.addRegularExpression("-", "MINUS");
    af.addRegularExpression("[0-9]+", "NUMBER");
    af.addRegularExpression("[0-9]+e[+-]?[0-9]+", "REAL");
    af.toAFD();
    AnalisadorLexico al(af);
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
