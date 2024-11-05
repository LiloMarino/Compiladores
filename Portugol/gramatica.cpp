#include "gramatica.hpp"
#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <deque>
#include <algorithm>

Gramatica::Gramatica() : last_id(0) {}

void Gramatica::addProduction(const std::string &nao_terminal, const std::string &produto)
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
    std::deque<std::string> symbols;
    std::stringstream ss(produto);
    std::string symbol;
    // Divide a produção em símbolos
    while (ss >> symbol)
    {
        symbols.push_back(symbol);
    }

    // Adiciona a produção à lista de produções associada ao id
    productions[id].emplace_back(std::move(symbols));
}

void Gramatica::toParsingTable(AnalisadorSintatico &sintatico)
{
    getGramaticaGroups();

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

void Gramatica::identifyConflicts()
{
    // Passo 1: Detectar Recursão à Esquerda Direta
    for (auto &[n_terminal, id] : nao_terminais)
    {
        bool has_direct_recursion = false;

        for (auto &production : productions[id])
        {
            if (n_terminal == production.front())
            {
                has_direct_recursion = true;
                std::cout << "Recursão à Esquerda Direta detectada em: " << n_terminal << " ->";
                for (const auto &symbol : production)
                    std::cout << " " << symbol;
                std::cout << std::endl;
            }
        }

        if (has_direct_recursion)
        {
            std::cout << "Sugestão: Eliminar a recursão à esquerda direta para o não-terminal " << n_terminal << "." << std::endl << std::endl;;
        }
    }

    // Passo 2: Detectar Recursão à Esquerda Indireta
    for (auto &[n_terminal, id] : nao_terminais)
    {
        std::set<std::string> visited;
        std::deque<std::string> stack;
        stack.push_back(n_terminal);

        while (!stack.empty())
        {
            std::string current = stack.front();
            stack.pop_front();

            if (visited.count(current) > 0)
            {
                std::cout << "Recursão à Esquerda Indireta detectada envolvendo: " << n_terminal << " -> " << current << std::endl;
                std::cout << "Sugestão: Eliminar a recursão à esquerda indireta." << std::endl << std::endl;;
                break;
            }

            visited.insert(current);

            for (auto &production : productions[nao_terminais[current]])
            {
                if (nao_terminais.count(production.front()) > 0)
                {
                    stack.push_back(production.front());
                }
            }
        }
    }

    // Passo 3: Detectar Fatoração à Esquerda
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

        // Detectar produções que precisam de fatoração à esquerda
        for (const auto &[prefix, group] : prefix_map)
        {
            if (group.size() > 1) // Mais de uma produção compartilha o mesmo prefixo
            {
                std::cout << "Fatoração à Esquerda necessária para o não-terminal " << n_terminal << " devido ao prefixo comum \"" << prefix << "\"." << std::endl;
                for (const auto &prod : group)
                {
                    std::cout << "  " << n_terminal << " ->";
                    for (const auto &sym : prod)
                        std::cout << " " << sym;
                    std::cout << std::endl;
                }
                std::cout << "Sugestão: Aplicar fatoração à esquerda para o não-terminal " << n_terminal << "." << std::endl << std::endl;;
            }
        }
    }
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
