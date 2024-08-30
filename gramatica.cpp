#include "gramatica.hpp"
#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <deque>
#include <algorithm>

Gramatica::Gramatica() : last_id(0) {}

bool Gramatica::addProduction(const std::string &nao_terminal, const std::string &produto)
{
    int id;
    bool added = false;

    // Verifica se o não-terminal já existe
    auto it = nao_terminais.find(nao_terminal);
    if (it == nao_terminais.end())
    {
        // Novo não-terminal, atribui um novo id
        id = ++last_id;
        nao_terminais[nao_terminal] = id;
        groups[id] = GramaticaGroup();
    }
    else
    {
        // Não-terminal já existe, usa o id existente
        id = it->second;
    }

    std::deque<std::string> symbols;
    std::stringstream ss(produto);
    std::string symbol;

    // Divide a produção em símbolos
    while (ss >> symbol)
    {
        symbols.push_back(symbol);
    }

    // Verifica se a produção já existe
    auto &prods = productions[id];
    if (std::find(prods.begin(), prods.end(), symbols) == prods.end())
    {
        // Adiciona a produção à lista de produções associada ao id
        prods.emplace_back(std::move(symbols));
        added = true;
    }

    return added;
}

void Gramatica::toParsingTable(AnalisadorSintatico &sintatico)
{
    toLL1();
    getGramaticaGroups();
    printGramaticaGroups();

    for (const auto &[id, prods] : productions)
    {
        const auto &n_terminal = getNaoTerminalById(id);

        for (auto &production : prods)
        {
            std::unordered_set<std::string> first = getFirst(n_terminal, production);
            for (auto &&token : first)
            {
                sintatico.addProduction(n_terminal, token, production);
            }
        }
    }
}

void Gramatica::toLL1()
{
    bool changed;

    do
    {
        changed = false;

        // Passo 1: Eliminar Recursão à Esquerda
        for (auto &[n_terminal, id] : nao_terminais)
        {
            std::vector<std::deque<std::string>> alpha_productions; // Produções recursivas à esquerda (A -> Aa)
            std::vector<std::deque<std::string>> beta_productions;  // Produções não-recursivas (A -> b)

            for (auto &production : productions[id])
            {
                if (n_terminal == production.front())
                {
                    // A produção é recursiva à esquerda
                    production.pop_front();
                    alpha_productions.push_back(production);
                }
                else
                {
                    // A produção não é recursiva à esquerda
                    beta_productions.push_back(production);
                }
            }

            // Se houver produções recursivas à esquerda, aplicar a transformação
            if (!alpha_productions.empty())
            {
                // Criar um novo não-terminal A'
                std::string new_non_terminal = n_terminal + "'"; // Pode adicionar uma verificação para garantir que não há conflito de nomes

                // Transformar as produções não-recursivas (A -> b) em A -> bA'
                for (auto &beta : beta_productions)
                {
                    std::string beta_prod = "";
                    for (const auto &symbol : beta)
                    {
                        beta_prod += symbol + " ";
                    }
                    beta_prod += new_non_terminal; // Adiciona A' no final

                    if (addProduction(n_terminal, beta_prod))
                    {
                        changed = true;
                    }
                }

                // Transformar as produções recursivas (A -> Aa) em A' -> aA'
                for (auto &alpha : alpha_productions)
                {
                    std::string alpha_prod = "";
                    for (const auto &symbol : alpha)
                    {
                        alpha_prod += symbol + " ";
                    }
                    alpha_prod += new_non_terminal; // Adiciona A' no final

                    if (addProduction(new_non_terminal, alpha_prod))
                    {
                        changed = true;
                    }
                }

                // Adicionar a produção ε (A' -> ε) ao novo não-terminal
                if (addProduction(new_non_terminal, ""))
                {
                    changed = true;
                }
            }
        }

        // Passo 2: Fatoração à Esquerda
        for (auto &[n_terminal, id] : nao_terminais)
        {
            std::map<std::string, std::vector<std::deque<std::string>>> prefix_map;

            // Organizar produções por prefixo comum
            for (auto &production : productions[id])
            {
                if (!production.empty())
                {
                    std::string prefix = production.front();
                    prefix_map[prefix].push_back(production);
                }
            }

            // Fatorar as produções que compartilham prefixo
            for (const auto &[prefix, group] : prefix_map)
            {
                if (group.size() > 1) // Mais de uma produção compartilha o mesmo prefixo
                {
                    // Criar um novo não-terminal A'
                    std::string new_non_terminal = n_terminal + prefix + "'";

                    // Adicionar a nova produção fatorada (A -> prefix A')
                    if (addProduction(n_terminal, prefix + " " + new_non_terminal))
                    {
                        changed = true;
                    }

                    // Adicionar as produções para o novo não-terminal A'
                    for (const auto &prod : group)
                    {
                        std::string suffix = "";
                        for (size_t i = 1; i < prod.size(); ++i) // Pula o prefixo
                        {
                            suffix += prod[i] + " ";
                        }
                        if (addProduction(new_non_terminal, suffix.empty() ? "" : suffix))
                        {
                            changed = true;
                        }
                    }
                }
            }
        }
    } while (changed);
}

std::unordered_set<std::string> Gramatica::getFirst(const std::string &n_terminal, const std::deque<std::string> &production)
{
    std::unordered_set<std::string> first_set;

    if (production.empty())
    {
        // Produção vazia então usa o FOLLOW
        GramaticaGroup group = groups[nao_terminais[n_terminal]];
        first_set.insert(group.follow.begin(), group.follow.end());
    }
    else
    {
        auto it = production.begin();
        while (it != production.end())
        {
            const std::string &symbol = *it;

            if (isNonTerminal(symbol))
            {
                // O símbolo é um não-terminal então verifica se é nullable
                GramaticaGroup group = groups[nao_terminais[symbol]];
                first_set.insert(group.first.begin(), group.first.end());

                // Se o não-terminal é nullable, continua a adicionar o FIRST dos símbolos subsequentes
                if (!group.nullable)
                {
                    break; // Sai do loop se o não-terminal não é nullable
                }
            }
            else
            {
                // É um terminal então seu FIRST é o próprio terminal
                first_set.insert(symbol);
                break;
            }

            ++it; // Avança para o próximo símbolo na produção
        }

        // Caso o FIRST do símbolo seja nullable, adicionar o FIRST dos símbolos subsequentes
        if (it == production.end())
        {
            // Se chegamos ao final da produção, considera o FOLLOW do não-terminal original
            GramaticaGroup group = groups[nao_terminais[n_terminal]];
            first_set.insert(group.follow.begin(), group.follow.end());
        }
    }

    return first_set;
}

std::string Gramatica::getNaoTerminalById(int id) const
{
    for (const auto &[nao_terminal, nao_terminal_id] : nao_terminais)
    {
        if (nao_terminal_id == id)
        {
            return nao_terminal;
        }
    }
    throw std::runtime_error("Erro de consistência nos dados da gramática");
}

bool Gramatica::isNonTerminal(const std::string &nao_terminal) const
{
    auto it = nao_terminais.find(nao_terminal);
    return (it != nao_terminais.end());
}

void Gramatica::getGramaticaGroups()
{
    bool changed;

    do
    {
        changed = false;

        // Para cada produção X -> Y1 Y2 ... Yk
        for (auto &[id, prods] : productions)
        {
            auto &group = groups[id];

            for (const auto &symbols : prods)
            {
                bool all_nullable = true;

                // Verifica Nullable e atualiza FIRST
                for (size_t i = 0; i < symbols.size(); ++i)
                {
                    const std::string &symbol = symbols[i];

                    if (!isNonTerminal(symbol))
                    {
                        // Se é um terminal, adiciona ao FIRST[X]
                        auto [it, inserted] = group.first.insert(symbol);
                        if (inserted)
                        {
                            changed = true;
                        }
                        all_nullable = false;
                        break;
                    }
                    else
                    {
                        // Se é um não-terminal, une FIRST[Yi] com FIRST[X]
                        int symbol_id = nao_terminais[symbol];
                        auto &symbol_group = groups[symbol_id];

                        size_t before_size = group.first.size();
                        group.first.insert(symbol_group.first.begin(), symbol_group.first.end());
                        if (group.first.size() > before_size)
                        {
                            changed = true;
                        }

                        if (!symbol_group.nullable)
                        {
                            all_nullable = false;
                            break;
                        }
                    }
                }

                // Atualiza Nullable[X]
                if (all_nullable && !group.nullable)
                {
                    group.nullable = true;
                    changed = true;
                }

                // Atualiza FOLLOW
                for (size_t i = 0; i < symbols.size(); ++i)
                {
                    const std::string &symbol = symbols[i];

                    if (isNonTerminal(symbol))
                    {
                        int symbol_id = nao_terminais[symbol];
                        auto &symbol_group = groups[symbol_id];

                        // FOLLOW[Yi] ← FOLLOW[Yi] U FOLLOW[X] se Yi+1 ... Yk são todos Nullable
                        bool nullable_suffix = true;
                        for (size_t j = i + 1; j < symbols.size(); ++j)
                        {
                            const std::string &next_symbol = symbols[j];

                            if (!isNonTerminal(next_symbol))
                            {
                                auto [it, inserted] = symbol_group.follow.insert(next_symbol);
                                if (inserted)
                                {
                                    changed = true;
                                }
                                nullable_suffix = false;
                                break;
                            }
                            else
                            {
                                int next_symbol_id = nao_terminais[next_symbol];
                                auto &next_group = groups[next_symbol_id];

                                size_t before_size = symbol_group.follow.size();
                                symbol_group.follow.insert(next_group.first.begin(), next_group.first.end());
                                if (symbol_group.follow.size() > before_size)
                                {
                                    changed = true;
                                }

                                if (!next_group.nullable)
                                {
                                    nullable_suffix = false;
                                    break;
                                }
                            }
                        }

                        if (nullable_suffix)
                        {
                            size_t before_size = symbol_group.follow.size();
                            symbol_group.follow.insert(group.follow.begin(), group.follow.end());
                            if (symbol_group.follow.size() > before_size)
                            {
                                changed = true;
                            }
                        }
                    }
                }
            }
        }
    } while (changed);
}

void Gramatica::printGramaticaGroups() const
{
    // Tamanho das colunas para formatação
    const int col_width = 15;

    std::cout << std::left
              << std::setw(col_width) << "Non-terminal"
              << std::setw(col_width) << "Nullable"
              << std::setw(col_width) << "First"
              << std::setw(col_width) << "Follow"
              << std::endl;

    std::cout << std::string(col_width * 4, '-') << std::endl;

    for (const auto &[nao_terminal, id] : nao_terminais)
    {
        const auto &group = groups.at(id);

        // Imprimindo Nullable
        std::cout << std::setw(col_width) << nao_terminal;
        std::cout << std::setw(col_width) << (group.nullable ? "true" : "false");

        // Imprimindo First
        std::string first_str = "{ ";
        for (const auto &sym : group.first)
        {
            first_str += sym + " ";
        }
        first_str += "}";
        std::cout << std::setw(col_width) << first_str;

        // Imprimindo Follow
        std::string follow_str = "{ ";
        for (const auto &sym : group.follow)
        {
            follow_str += sym + " ";
        }
        follow_str += "}";
        std::cout << std::setw(col_width) << follow_str;

        std::cout << std::endl;
    }
}

void Gramatica::printGramatica() const
{
    for (const auto &[n_terminal, id] : nao_terminais)
    {
        const auto &prods = productions.at(id);
        for (const auto &prod : prods)
        {
            std::cout << n_terminal << " -> ";

            for (const auto &symbol : prod)
            {
                std::cout << symbol << " ";
            }

            std::cout << std::endl;
        }
    }
}
