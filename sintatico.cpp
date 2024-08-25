#include "sintatico.hpp"
#include <iostream>
#include <iomanip>

AnalisadorSintatico::AnalisadorSintatico(std::string simbolo_inicial) : simbolo_inicial(simbolo_inicial)
{
}

void AnalisadorSintatico::addProduction(const std::string &nao_terminal, const std::string &terminal, const std::string &produto)
{
    adicionarNaoTerminal(nao_terminal);
    adicionarTerminal(terminal);

    int row = nao_terminais[nao_terminal];
    int col = terminais[terminal];

    parsing_table[row][col] = SintaticGroup(nao_terminal, produto);
}

void AnalisadorSintatico::analisar(const std::list<LexicalGroup> &tokens)
{
    std::stack<std::string> pilha;
    pilha.push(simbolo_inicial);
}

void AnalisadorSintatico::exibirTabela(std::ostream &output) const
{
    // Cabeçalho com terminais
    output << std::setw(2) << " ";
    for (const auto &[terminal, index] : terminais)
    {
        output << std::setw(15) << terminal;
    }
    output << std::endl;

    // Linhas com não-terminais e produções
    for (const auto &[nao_terminal, row] : nao_terminais)
    {
        output << std::setw(2) << nao_terminal;
        for (size_t col = 0; col < terminais.size(); ++col)
        {
            const SintaticGroup &producao = parsing_table[row][col];
            if (!producao.isEmpty())
            {
                std::string producaoStr = producao.pop + " -> " + producao.push;
                output << std::setw(15) << producaoStr;
            }
            else
            {
                output << std::setw(15) << "-";
            }
        }
        output << std::endl;
    }
}

SintaticGroup AnalisadorSintatico::getProduction(const std::string &nao_terminal, const std::string &terminal) const
{
    auto nt_it = nao_terminais.find(nao_terminal);
    auto t_it = terminais.find(terminal);

    if (nt_it != nao_terminais.end() && t_it != terminais.end())
    {
        int row = nt_it->second;
        int col = t_it->second;
        return parsing_table[row][col];
    }
    else
    {
        throw std::runtime_error("Não-terminal ou terminal não encontrado na tabela.");
    }
}

void AnalisadorSintatico::adicionarNaoTerminal(const std::string &nao_terminal)
{
    if (nao_terminais.find(nao_terminal) == nao_terminais.end())
    {
        int index = nao_terminais.size();
        nao_terminais[nao_terminal] = index;

        // Ajusta o tamanho da matriz adicionando uma nova linha
        parsing_table.resize(nao_terminais.size());
        parsing_table.back().resize(terminais.size());
    }
}

void AnalisadorSintatico::adicionarTerminal(const std::string &terminal)
{
    if (terminais.find(terminal) == terminais.end())
    {
        int index = terminais.size();
        terminais[terminal] = index;
        // Ajusta o tamanho de cada linha na matriz adicionando uma nova coluna
        for (auto &row : parsing_table)
        {
            row.resize(terminais.size());
        }
    }
}

const char *AnalisadorSintatico::SintaticError::what() const noexcept
{
    return mensagem.c_str();
}
