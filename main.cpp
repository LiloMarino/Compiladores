#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include "automato_finito.hpp"
#include "lexico.hpp"

using namespace std;

void createAutomato(AutomatoFinito &af)
{
    af.addRegularExpression("if", "IF");
    af.addRegularExpression("then", "THEN");
    af.addRegularExpression("else", "ELSE");
    af.addRegularExpression("begin", "BEGIN");
    af.addRegularExpression("end", "END");
    af.addRegularExpression("print", "PRINT");
    af.addRegularExpression(";", "SEMI");
    af.addRegularExpression("[+-]?[0-9]+", "NUM");
    af.addRegularExpression("=", "EQ");
    af.toAFD();
}

int main()
{
    AutomatoFinito af;
    createAutomato(af);
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
