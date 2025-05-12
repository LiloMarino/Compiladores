#include <iostream>
#include <string>
#include <list>
#include "automato.hpp"
#include "lexico.hpp"

using namespace std;
void createAutomato(Automato &au)
{
    au.addTransitions(1, 2, "i");
    au.addTransitions(1, 4, "a-h,j-z");
    au.addTransitions(1, 5, ".");
    au.addTransitions(1, 7, "0-9");
    au.addTransitions(1, 9, "-");
    au.addTransitions(1, 12, " ");
    au.addTransitions(1, 13, "other");
    au.addTransitions(2, 3, "f");
    au.addTransitions(2, 4, "a-e,g-z,0-9");
    au.addTransitions(3, 4, "0-9,a-z");
    au.addTransitions(4, 4, "0-9,a-z");
    au.addTransitions(5, 6, "0-9");
    au.addTransitions(6, 6, "0-9");
    au.addTransitions(7,7, "0-9");
    au.addTransitions(7,8, ".");
    au.addTransitions(8,8, "0-9");
    au.addTransitions(9,10, "-");
    au.addTransitions(10,10, "a-z");
    au.addTransitions(10,11, "\n");
    au.addTransitions(12,12, " ");
}

void createAnalisador(AnalisadorLexico &al)
{
    al.addToken("ID", 2);
    al.addToken("IF", 3);
    al.addToken("ID", 4);
    al.addToken("error", 5);
    al.addToken("REAL", 6);
    al.addToken("NUM", 7);
    al.addToken("REAL", 8);
    al.addToken("error", 9);
    al.addToken("comment", 11);
    al.addToken("white space", 12);
    al.addToken("error", 13);
}

int main()
{
    Automato a(13);
    createAutomato(a);
    AnalisadorLexico al(a);
    createAnalisador(al);
    string input;
    list<recon> tokens_reconhecidos;
    while (getline(cin, input))
    {
        input += "\n";
        list<recon> aux = al.reconhecer(input);
        tokens_reconhecidos.splice(tokens_reconhecidos.end(), aux);
    }
    for (recon &token : tokens_reconhecidos)
    {
        cout << token.cadeia << " " << token.token << endl;
    }
    return 0;
}
