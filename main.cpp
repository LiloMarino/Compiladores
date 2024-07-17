#include <iostream>
#include <string>
#include <list>
#include "automato.hpp"
#include "reconhecedor.hpp"

using namespace std;
void createAutomato(Automato &au)
{
    au.addTransitions(1,2,"a-z");
    au.addTransitions(1,3,"+");
    au.addTransitions(1,4,"-");
    au.addTransitions(1,5,"0-9");
    au.addTransitions(5,5,"0-9");
    au.addTransitions(5,6,"e");
    au.addTransitions(6,8,"0-9");
    au.addTransitions(6,7,"+,-");
    au.addTransitions(7,8,"0-9");
    au.addTransitions(8,8,"0-9");
}

void createReconhecedor(Reconhecedor &re)
{
    re.addToken("F",2);
    re.addToken("F",3);
    re.addToken("F",4);
    re.addToken("F",5);
    re.addToken("F",8);
}

int main()
{
    Automato a(8);
    createAutomato(a);
    Reconhecedor r(a);
    createReconhecedor(r);
    string input;
    list<string> entradas;
    list<string> tokens;
    while (getline(cin, input))
    {
        entradas.push_back(input);
        list<string> tokens_reconhecidos = r.reconhecer(input);
        tokens.splice(tokens.end(), tokens_reconhecidos);
    }
    for (const auto &token : tokens)
    {
        cout << token;
    }
    for (const auto &entrada : entradas)
    {
        cout << entrada;
    }

    return 0;
}
