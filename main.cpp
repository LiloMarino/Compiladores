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
    af.addRegularExpression("[a-zA-Z_-_][a-zA-Z0-9_-_]*", "identificador");
    af.addRegularExpression("[0-9]+", "inteiro");
    af.addRegularExpression("[0-9]+\\.[0-9]+", "real");
    af.addRegularExpression("\"[a-zA-Z0-9_-_]*\"", "string");
    af.addRegularExpression("//.*", "comment");
    af.addRegularExpression("{", "multiline start");
    af.addRegularExpression("}", "multiline end");
    af.toAFD();
}

void createParseTable(AnalisadorSintatico &as)
{
}

int main()
{
    AutomatoFinito af;
    createAutomato(af);
    AnalisadorLexico lexic(af);
    lexic.addIgnoreSymbol(' ');
    lexic.setMultilineComment("multiline start", "multiline end");
    AnalisadorSintatico sintatic("S");
    createParseTable(sintatic);
    string input;
    bool first_line = true;
    while (getline(cin, input))
    {
        try
        {
            list<LexicalGroup> tokens = lexic.reconhecer(input);
            for (auto &&[token, cadeia] : tokens)
            {
                std::cout << token << " " << cadeia << std::endl;
            }

            // sintatic.analisar(tokens);
            // if (!first_line)
            // {
            //     std::cout << std::endl;
            // }
            // std::cout << "CADEIA ACEITA";
            first_line = false;
        }
        catch (const AnalisadorSintatico::SintaticError &e)
        {
            if (!first_line)
            {
                std::cout << std::endl;
            }
            std::cout << e.what();
            first_line = false;
        }
        catch (const AnalisadorLexico::LexicalError &e)
        {
            if (!first_line)
            {
                std::cout << std::endl;
            }
            std::cout << e.what();
            first_line = false;
        }
    }
    return 0;
}