#include <iostream>
#include <string>
#include <list>
#include "automato.hpp"
#include "lexico.hpp"

using namespace std;
void createAutomato(Automato &au)
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
    Automato a(8);
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
    for (recon &token : tokens_reconhecidos)
    {
        if (token.token != "ERRO")
        {
            cout << token.cadeia << endl;
        }
        else
        {
            cout << "ERRO" << endl;
        }
    }
    return 0;
}
