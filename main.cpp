#include <iostream>
#include <string>
#include <list>
#include "automato.hpp"
#include "lexico.hpp"

using namespace std;
void createAutomato(Automato &au)
{
    au.addTransitions(1, 17, "+");
    au.addTransitions(1, 18, "-");
    au.addTransitions(1, 19, "0-9");
    au.addTransitions(1, 2, "h");
    au.addTransitions(1, 4, "s");
    au.addTransitions(1, 11, "c");
    au.addTransitions(19, 19, "0-9");
    au.addTransitions(19, 20, "e");
    au.addTransitions(2, 3, "e");
    au.addTransitions(2, 7, "i");
    au.addTransitions(4, 5, "h");
    au.addTransitions(11, 12, "a");
    au.addTransitions(20, 22, "0-9");
    au.addTransitions(20, 21, "+,-");
    au.addTransitions(3, 9, "r");
    au.addTransitions(7, 8, "s");
    au.addTransitions(5, 6, "e");
    au.addTransitions(12, 13, "t");
    au.addTransitions(12, 14, "r");
    au.addTransitions(21, 22, "0-9");
    au.addTransitions(9, 10, "s");
    au.addTransitions(13, 15, "s");
    au.addTransitions(14, 16, "s");
    au.addTransitions(22, 22, "0-9");
}

void createAnalisador(AnalisadorLexico &al)
{
    al.addToken("MAIS", 17);
    al.addToken("MENOS", 18);
    al.addToken("INTEIRO", 19);
    al.addToken("ELE", 3);
    al.addToken("DELE", 8);
    al.addToken("DELA", 10);
    al.addToken("REAL", 22);
    al.addToken("ELA", 6);
    al.addToken("GATO", 13);
    al.addToken("GATOS", 15);
    al.addToken("CARRO", 14);
    al.addToken("CARROS", 16);
}

int main()
{
    Automato a(22);
    createAutomato(a);
    AnalisadorLexico al(a);
    createAnalisador(al);
    string input;
    list<recon> tokens_reconhecidos;
    while (getline(cin, input))
    {
        list<recon> aux = al.reconhecer(input);
        tokens_reconhecidos.splice(tokens_reconhecidos.end(), aux);
    }
    for (auto it = tokens_reconhecidos.begin(); it != tokens_reconhecidos.end(); ++it)
    {
        recon token = *it;
        cout << token.token << ((token.token == "INTEIRO" || token.token == "REAL") ? " " + token.cadeia : "");
        if (next(it) != tokens_reconhecidos.end())
        {
            cout << endl;
        }
        
    }
    return 0;
}
