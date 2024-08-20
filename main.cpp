#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include "automato_finito.hpp"
#include "lexico.hpp"

using namespace std;

void createAutomato(AutomatoFinito &au)
{
    au.addTransitions(1, 2, "c");
    au.addTransitions(2, 3, "a");
    au.addTransitions(3, 4, "t");
    au.addTransitions(3, 5, "r");
    au.addTransitions(4, 6, "s");
    au.addTransitions(5, 7, "s");
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
    std::ofstream arquivo("tabela_transicao.txt");
    af.addRegularExpression("cat");
    af.addRegularExpression("cats");
    af.addRegularExpression("car");
    af.addRegularExpression("cars");
    af.printTransitionTable(arquivo);
    af.printVisualizacaoDOT("arq1.dot");
    arquivo.close();
    arquivo.open("tabela_transicao2.txt");
    af.toAFD();
    af.printTransitionTable(arquivo);
    af.printVisualizacaoDOT("arq2.dot");
    arquivo.close();
    arquivo.open("tabela_transicao3.txt");
    afd.printTransitionTable(arquivo);
    afd.printVisualizacaoDOT("arq3.dot");
    arquivo.close();
    cout << (af == afd);
    AnalisadorLexico al(afd);
    createAnalisador(al);
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
