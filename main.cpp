#include <iostream>
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

void createAnalisador(AnalisadorLexico &al)
{
    al.addToken("2", 2);
    al.addToken("3", 3);
    al.addToken("4", 4);
    al.addToken("5", 5);
    al.addToken("8", 8);
}

int main()
{
    AutomatoFinito afd(8);
    createAutomato(afd);
    AutomatoFinito af;
    af.addRegularExpression("[a-z][a-z0-9]*");
    af.addRegularExpression("+");
    af.addRegularExpression("-");
    af.addRegularExpression("[0-9]+");
    af.addRegularExpression("[0-9]+e[+-]?[0-9]+");
    cout << (af == afd);
    AnalisadorLexico al(afd);
    createAnalisador(al);
    string input;
    while (getline(cin, input))
    {
        input += "\n";
        list<recon> aux = al.reconhecer(input);
        for (recon &token : aux)
        {
            cout << token.cadeia << " " << token.token << endl;
        }
    }
    return 0;
}
