#include "sintatico.hpp"
#include <iostream>
#include <iomanip>
#include <map>

AnalisadorSintatico::AnalisadorSintatico(std::string simbolo_inicial) : simbolo_inicial(simbolo_inicial)
{
}

void AnalisadorSintatico::addProduction(const std::string &nao_terminal, const std::string &terminal, const std::string &produto)
{
    adicionarNaoTerminal(nao_terminal);
    adicionarTerminal(terminal);

    int row = nao_terminais[nao_terminal];
    int col = terminais[terminal];

    std::deque<std::string> simbols;

    // Dividindo a produção em símbolos separados
    std::stringstream ss(produto);
    std::string symbol;
    while (ss >> symbol)
    {
        simbols.push_back(symbol);
    }

    parsing_table[row][col] = SintaticGroup(nao_terminal, simbols);
}

void AnalisadorSintatico::analisar(const std::list<LexicalGroup> &tokens)
{
    std::deque<std::string> pilha;
    pilha.push_back(simbolo_inicial);
    std::string debug;
    for (auto &[token, str] : tokens)
    {
        bool obtido_token = false;
        while (!obtido_token)
        {
            debug.clear();
            for (auto it = pilha.rbegin(); it != pilha.rend(); ++it)
            {
                debug += *it;
            }
            std::cout << debug << std::endl;
            std::string n_terminal = pilha.back();
            pilha.pop_back();
            if (n_terminal != token)
            {
                // Não terminal obtido então deriva
                SintaticGroup production = this->getProduction(n_terminal, token);
                if (production.isEmpty())
                {
                    std::string expected = getExpected(n_terminal);
                    throw SintaticError(token, expected);
                }
                for (auto it = production.simbols.rbegin(); it != production.simbols.rend(); ++it)
                {
                    pilha.push_back(std::move(*it));
                }
            }
            else
            {
                // Token obtido
                obtido_token = true;
            }
        }
    }
    debug.clear();
    for (auto it = pilha.rbegin(); it != pilha.rend(); ++it)
    {
        debug += *it;
    }
    std::cout << debug << std::endl;
    if (!pilha.empty())
    {
        std::string expected = getExpected(pilha.back());
        throw SintaticError("", expected);
    }
}

std::string AnalisadorSintatico::getExpected(std::string &n_terminal)
{
    std::string expected;

    for (auto it_terminais = terminais.begin(); it_terminais != terminais.end(); ++it_terminais)
    {
        // Se a produção não está vazia
        if (!this->getProduction(n_terminal, it_terminais->first).isEmpty())
        {
            if (expected.empty())
            {
                expected = it_terminais->first; // String do terminal
            }
            else
            {
                expected += ", " + it_terminais->first; // String do terminal
            }
        }
    }
    return expected;
}

void AnalisadorSintatico::exibirTabela(std::ostream &output) const
{
    // Ordena os não-terminais e terminais
    std::map<std::string, int> ordenado_nao_terminais(nao_terminais.begin(), nao_terminais.end());
    std::map<std::string, int> ordenado_terminais(terminais.begin(), terminais.end());

    // Cabeçalho com terminais
    output << std::setw(2) << " ";
    for (const auto &[terminal, index] : ordenado_terminais)
    {
        output << std::setw(15) << terminal;
    }
    output << std::endl;

    // Linhas com não-terminais e produções
    for (const auto &[nao_terminal, row] : ordenado_nao_terminais)
    {
        output << std::setw(2) << nao_terminal;
        for (const auto &[terminal, col] : ordenado_terminais)
        {
            const SintaticGroup &producao = parsing_table[row][col];
            if (!producao.isEmpty())
            {
                std::string producaoStr = producao.pop + " -> ";
                for (const auto &simbol : producao.simbols)
                {
                    producaoStr += simbol;
                }
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

AnalisadorSintatico::SintaticError::SintaticError(const std::string &token, const std::string &expected)
{
    std::string erro;
    erro += "ERRO SINTATICO EM: ";
    erro += token;
    erro += " ESPERADO: ";
    erro += expected;
    this->mensagem = std::move(erro);
}

AnalisadorSintatico::SintaticError::SintaticError(const std::string &msg)
{
    std::string erro;
    erro += "ERRO SINTATICO: ";
    erro += msg;
    this->mensagem = std::move(erro);
}

const char *AnalisadorSintatico::SintaticError::what() const noexcept
{
    return mensagem.c_str();
}
