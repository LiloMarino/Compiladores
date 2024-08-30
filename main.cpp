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
    af.addRegularExpression(";", ";");
    af.addRegularExpression(",", ",");
    af.addRegularExpression(":", ":");
    af.addRegularExpression("\\.", ".");
    af.addRegularExpression("\\[", "[");
    af.addRegularExpression("\\]", "]");
    af.addRegularExpression("(", "(");
    af.addRegularExpression(")", ")");
    af.addRegularExpression("=", "=");
    af.addRegularExpression("<>", "<>");
    af.addRegularExpression(">", ">");
    af.addRegularExpression(">=", ">=");
    af.addRegularExpression("<", "<");
    af.addRegularExpression("<=", "<=");
    af.addRegularExpression("+", "+");
    af.addRegularExpression("-", "-");
    af.addRegularExpression("*", "*");
    af.addRegularExpression("/", "/");
    af.addRegularExpression("<-", "<-");
    af.addRegularExpression("[a-zA-Z_-_][a-zA-Z0-9_-_]*", "identificador");
    af.addRegularExpression("[0-9]+", "numero_inteiro");
    af.addRegularExpression("[0-9]+\\.[0-9]+", "numero_real");
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
    grammar.addProduction("Programa", "algoritmo identificador ; BlocoVariaveis ProcedimentoFuncao BlocoComandos .");
    grammar.addProduction("ProcedimentoFuncao", "DeclaraProcedimento ProcedimentoFuncao");
    grammar.addProduction("ProcedimentoFuncao", "DeclaraFuncao ProcedimentoFuncao");
    grammar.addProduction("ProcedimentoFuncao", "");
    grammar.addProduction("DeclaraProcedimento", "procedimento identificador Parametros ; DeclaraParametros BlocoVariaveis BlocoComandos ;");
    grammar.addProduction("DeclaraFuncao", "funcao identificador Parametros : TipoBasico ; DeclaraParametros BlocoVariaveis BlocoComandos ;");
    grammar.addProduction("Parametros", "( DeclaraIdentificador )");
    grammar.addProduction("Parametros", "");
    grammar.addProduction("DeclaraParametros", "Declaracoes");
    grammar.addProduction("DeclaraParametros", "");
    grammar.addProduction("BlocoVariaveis", "variaveis Declaracoes");
    grammar.addProduction("BlocoVariaveis", "");
    grammar.addProduction("Declaracoes", "DeclaraVariaveis");
    grammar.addProduction("Declaracoes", "DeclaraTipo");
    grammar.addProduction("Declaracoes", "DeclaraVariaveis Declaracoes");
    grammar.addProduction("Declaracoes", "DeclaraTipo Declaracoes");
    grammar.addProduction("DeclaraTipo", "tipo identificador = VetorMatriz [ Dimensao ] TipoBasico ;");
    grammar.addProduction("DeclaraVariaveis", "TipoBasico : DeclaraIdentificador ;");
    grammar.addProduction("DeclaraIdentificador", "identificador");
    grammar.addProduction("DeclaraIdentificador", "identificador , DeclaraIdentificador");
    grammar.addProduction("VetorMatriz", "vetor");
    grammar.addProduction("VetorMatriz", "matriz");
    grammar.addProduction("Dimensao", "numero_inteiro : numero_inteiro");
    grammar.addProduction("Dimensao", "numero_inteiro : numero_inteiro , Dimensao");
    grammar.addProduction("TipoBasico", "inteiro");
    grammar.addProduction("TipoBasico", "real");
    grammar.addProduction("TipoBasico", "caractere");
    grammar.addProduction("TipoBasico", "logico");
    grammar.addProduction("TipoBasico", "identificador");
    grammar.addProduction("BlocoComandos", "inicio ListaComandos fim");
    grammar.addProduction("ListaComandos", "Comandos ;");
    grammar.addProduction("ListaComandos", "Comandos ; ListaComandos");
    grammar.addProduction("Comandos", "identificador");
    grammar.addProduction("Comandos", "identificador ( ExprIter )");
    grammar.addProduction("Comandos", "Variavel <- Expressao");
    grammar.addProduction("Comandos", "se Expressao entao ListaComandos fim se");
    grammar.addProduction("Comandos", "se Expressao entao ListaComandos senao ListaComandos fim se");
    grammar.addProduction("Comandos", "enquanto Expressao faca ListaComandos fim enquanto");
    grammar.addProduction("Comandos", "para identificador de Expressao ate Expressao faca ListaComandos fim para");
    grammar.addProduction("Comandos", "para identificador de Expressao ate Expressao passo Expressao faca ListaComandos fim para");
    grammar.addProduction("Comandos", "repita ListaComandos ate Expressao");
    grammar.addProduction("Comandos", "leia ( Variavel )");
    grammar.addProduction("Comandos", "imprima ( ExprIter )");
    grammar.addProduction("Expressao", "ExpressaoSimples");
    grammar.addProduction("Expressao", "Expressao = ExpressaoSimples");
    grammar.addProduction("Expressao", "Expressao <> ExpressaoSimples");
    grammar.addProduction("Expressao", "Expressao < ExpressaoSimples");
    grammar.addProduction("Expressao", "Expressao <= ExpressaoSimples");
    grammar.addProduction("Expressao", "Expressao >= ExpressaoSimples");
    grammar.addProduction("Expressao", "Expressao > ExpressaoSimples");
    grammar.addProduction("ExpressaoSimples", "Termo");
    grammar.addProduction("ExpressaoSimples", "+ Termo");
    grammar.addProduction("ExpressaoSimples", "- Termo");
    grammar.addProduction("ExpressaoSimples", "ExpressaoSimples + Termo");
    grammar.addProduction("ExpressaoSimples", "ExpressaoSimples - Termo");
    grammar.addProduction("ExpressaoSimples", "ExpressaoSimples ou Termo");
    grammar.addProduction("Termo", "Fator");
    grammar.addProduction("Termo", "Termo * Fator");
    grammar.addProduction("Termo", "Termo / Fator");
    grammar.addProduction("Termo", "Termo div Fator");
    grammar.addProduction("Termo", "Termo e Fator");
    grammar.addProduction("Fator", "( Expressao )");
    grammar.addProduction("Fator", "nao Fator");
    grammar.addProduction("Fator", "numero_inteiro");
    grammar.addProduction("Fator", "numero_real");
    grammar.addProduction("Fator", "verdadeiro");
    grammar.addProduction("Fator", "falso");
    grammar.addProduction("Fator", "string");
    grammar.addProduction("Fator", "identificador ( ExprIter )");
    grammar.addProduction("Fator", "Variavel");
    grammar.addProduction("Variavel", "identificador");
    grammar.addProduction("Variavel", "identificador [ ExprIter ]");
    grammar.addProduction("ExprIter", "Expressao");
    grammar.addProduction("ExprIter", "Expressao , ExprIter");
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
    ofstream arquivo("__table.txt");
    sintatic.exibirTabela(arquivo);
    arquivo.close();
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