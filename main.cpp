#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include "automato_finito.hpp"
#include "lexico.hpp"
#include "sintatico.hpp"
#include "gramatica.hpp"

using namespace std;

void createAutomato(AutomatoFinito &af)
{
    af.addRegularExpression("algoritmo", "algoritmo", true);
    af.addRegularExpression("vetor", "vetor", true);
    af.addRegularExpression("enquanto", "enquanto", true);
    af.addRegularExpression("imprima", "imprima", true);
    af.addRegularExpression("inicio", "inicio", true);
    af.addRegularExpression("matriz", "matriz", true);
    af.addRegularExpression("faca", "faca", true);
    af.addRegularExpression("verdadeiro", "verdadeiro", true);
    af.addRegularExpression("fim", "fim", true);
    af.addRegularExpression("tipo", "tipo", true);
    af.addRegularExpression("para", "para", true);
    af.addRegularExpression("falso", "falso", true);
    af.addRegularExpression("variaveis", "variaveis", true);
    af.addRegularExpression("funcao", "funcao", true);
    af.addRegularExpression("de", "de", true);
    af.addRegularExpression("e", "e", true);
    af.addRegularExpression("inteiro", "inteiro", true);
    af.addRegularExpression("procedimento", "procedimento", true);
    af.addRegularExpression("ate", "ate", true);
    af.addRegularExpression("ou", "ou", true);
    af.addRegularExpression("real", "real", true);
    af.addRegularExpression("se", "se", true);
    af.addRegularExpression("passo", "passo", true);
    af.addRegularExpression("nao", "nao", true);
    af.addRegularExpression("caractere", "caractere", true);
    af.addRegularExpression("entao", "entao", true);
    af.addRegularExpression("repita", "repita", true);
    af.addRegularExpression("div", "div", true);
    af.addRegularExpression("logico", "logico", true);
    af.addRegularExpression("senao", "senao", true);
    af.addRegularExpression("leia", "leia", true);
    af.addRegularExpression(";", "ponto e virgula");
    af.addRegularExpression(",", "virgula");
    af.addRegularExpression(":", "dois pontos");
    af.addRegularExpression("\\.", "ponto");
    af.addRegularExpression("\\[", "abre colchetes");
    af.addRegularExpression("\\]", "fecha colchetes");
    af.addRegularExpression("(", "abre parenteses");
    af.addRegularExpression(")", "fecha parenteses");
    af.addRegularExpression("=", "igual");
    af.addRegularExpression("<>", "diferente");
    af.addRegularExpression(">", "maior");
    af.addRegularExpression(">=", "maior igual");
    af.addRegularExpression("<", "menor");
    af.addRegularExpression("<=", "menor igual");
    af.addRegularExpression("+", "mais");
    af.addRegularExpression("-", "menos");
    af.addRegularExpression("*", "vezes");
    af.addRegularExpression("/", "divisao");
    af.addRegularExpression("<-", "atribuicao");
    af.addRegularExpression("[a-zA-Z_-_][a-zA-Z0-9_-_]*", "identificador");
    af.addRegularExpression("[0-9]+", "number inteiro");
    af.addRegularExpression("[0-9]+\\.[0-9]+", "number real");
    af.addRegularExpression("\"[a-zA-Z0-9_-_]*\"", "string");
    af.addRegularExpression("//.*", "comment");
    af.addRegularExpression("{", "multiline start");
    af.addRegularExpression("}", "multiline end");
    af.addRegularExpression("[ ]+", "whitespace");
    af.toAFD();
}

void createParseTable(AnalisadorSintatico &as)
{
    Gramatica grammar;
    grammar.addProduction("Z","d");
    grammar.addProduction("Z","X Y Z");
    grammar.addProduction("Y","");
    grammar.addProduction("Y","c");
    grammar.addProduction("X","Y");
    grammar.addProduction("X","a");
    grammar.toParsingTable(as);
}

int main()
{
    AutomatoFinito af;
    createAutomato(af);
    AnalisadorLexico lexic(af);
    lexic.addIgnoreToken("whitespace");
    lexic.addIgnoreToken("comment");
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