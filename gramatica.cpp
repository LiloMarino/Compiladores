#include "gramatica.hpp"
#include <iostream>
#include <iomanip>

Gramatica::Gramatica() : last_id(0) {}

void Gramatica::addProduction(const std::string &nao_terminal, const std::string &production)
{
    int id;
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

    // Adiciona a produção à lista de produções associada ao id
    productions[id].push_back(production);
}

void Gramatica::toParsingTable(AnalisadorSintatico &sintatico)
{
    getGramaticaGroups();
    printGramaticaGroups();
}

bool Gramatica::isNonTerminal(const std::string &nao_terminal)
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

        // Para cada produção X -> Y1Y2...Yk
        for (auto &[id, prods] : productions)
        {
            auto &group = groups[id];

            for (const auto &production : prods)
            {
                bool all_nullable = true;

                // Verifica Nullable e atualiza FIRST
                for (size_t i = 0; i < production.size(); ++i)
                {
                    std::string symbol(1, production[i]);

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
                for (size_t i = 0; i < production.size(); ++i)
                {
                    std::string symbol(1, production[i]);

                    if (isNonTerminal(symbol))
                    {
                        int symbol_id = nao_terminais[symbol];
                        auto &symbol_group = groups[symbol_id];

                        // FOLLOW[Yi] ← FOLLOW[Yi] U FOLLOW[X] se Yi+1 ... Yk são todos Nullable
                        bool nullable_suffix = true;
                        for (size_t j = i + 1; j < production.size(); ++j)
                        {
                            std::string next_symbol(1, production[j]);

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
