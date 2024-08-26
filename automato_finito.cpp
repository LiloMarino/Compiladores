#include "automato_finito.hpp"
#include <string>
#include <stdexcept>
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <fstream>
#include <map>

AutomatoFinito::AutomatoFinito() : num_estados(0), deterministico(false), afnd(nullptr)
{
    afnd = new GenericAutomata();
}

AutomatoFinito::AutomatoFinito(int num_estados) : deterministico(true), afnd(nullptr)
{
    num_estados++; // Adiciona o estado de erro
    this->num_estados = num_estados;
    matriz.resize(num_estados);
    for (auto &linha : matriz)
    {
        linha.fill(0);
    }
}

AutomatoFinito::~AutomatoFinito()
{
    if (afnd != nullptr)
    {
        delete afnd;
    }
}

void AutomatoFinito::addTransitions(const int estado_inicial, const int estado_final, const std::string &transitions)
{
    if ((estado_inicial >= num_estados || estado_inicial < 0) ||
        (estado_final >= num_estados || estado_final < 0))
    {
        throw std::length_error("Estados Inválidos");
    }

    if (transitions == "other")
    {
        // Qualquer outra transição não definida (0)
        for (int i = 0; i < ASCII_SIZE; ++i)
        {
            if (matriz[estado_inicial][i] == 0)
            {
                matriz[estado_inicial][i] = estado_final;
            }
        }
    }
    else
    {
        bool intervalo = false;
        char inicio, fim;
        for (size_t i = 0; i < transitions.length(); i++)
        {
            const char atual = transitions[i];
            if (atual == ',')
            {
                // Cria a transição
                if (intervalo)
                {
                    for (char i = inicio; i <= fim; ++i)
                    {
                        matriz[estado_inicial][(int)i] = estado_final;
                    }
                    intervalo = false;
                }
                else
                {
                    matriz[estado_inicial][(int)fim] = estado_final;
                }
            }
            else if (atual == '-' && i + 1 < transitions.length())
            {
                // Salva o início e determina a leitura de um intervalo
                inicio = fim;
                intervalo = true;
            }
            fim = atual;
        }
        // Cria a última transição
        if (intervalo)
        {
            for (char i = inicio; i <= fim; ++i)
            {
                matriz[estado_inicial][(int)i] = estado_final;
            }
            intervalo = false;
        }
        else
        {
            matriz[estado_inicial][(int)fim] = estado_final;
        }
    }
}

void AutomatoFinito::addRegularExpression(const std::string &re, const std::string &token, bool ignore_case)
{
    if (this->deterministico)
    {
        throw std::logic_error("O Autômato é determinístico");
    }
    std::string regex;
    if (ignore_case)
    {
        for (char c : re)
        {
            regex += '[';
            regex += std::tolower(c);
            regex += std::toupper(c);
            regex += ']';
        }
    }
    else
    {
        regex = re;
    }
    int estado_final = this->afnd->addRegularExpression(std::move(regex));
    this->tokens.addToken(token);
    this->tokens.setFinalState(token, estado_final);
}

void AutomatoFinito::minimizeAFD()
{
    if (!this->deterministico)
    {
        throw std::logic_error("O Autômato não é determinístico");
    }

    const size_t tamanho = static_cast<size_t>(num_estados);

    // Inicializa a matriz de equivalência
    std::vector<std::vector<bool>> equivalencia(tamanho, std::vector<bool>(tamanho, true));

    // Marca estados não equivalentes com base nos estados finais e tokens
    for (size_t i = 1; i < tamanho; ++i)
    {
        if (this->tokens.isFinalState(i))
        {
            for (size_t j = 0; j < i; ++j)
            {
                if (!this->tokens.isFinalState(j))
                {
                    equivalencia[i][j] = false;
                    equivalencia[j][i] = false;
                }
                else
                {
                    // Verifica se reconhecem o mesmo token
                    if (this->tokens.getTokenIdByFinalState(i) != this->tokens.getTokenIdByFinalState(j))
                    {
                        equivalencia[i][j] = false;
                        equivalencia[j][i] = false;
                    }
                }
            }
        }
    }

    // Resolve as equivalências não triviais
    this->resolveEquivalencies(tamanho, equivalencia);

    // Mapeia os estados antigos para os novos estados minimizados
    std::vector<int> estado_minimizado(tamanho, -1);
    int estado_atual = 0;

    // Atualiza o mapeamento de estados
    for (size_t i = 0; i < tamanho; ++i)
    {
        if (estado_minimizado[i] == -1)
        {
            // Atribui o novo estado minimizado ao grupo de estados equivalentes
            estado_minimizado[i] = estado_atual;
            for (size_t j = 0; j < tamanho; ++j)
            {
                if (equivalencia[i][j] && estado_minimizado[j] == -1)
                {
                    estado_minimizado[j] = estado_atual;
                }
            }
            ++estado_atual;
        }
    }

    // Cria uma nova matriz para os estados minimizados
    std::vector<std::array<int, ASCII_SIZE>> nova_matriz(estado_atual, std::array<int, ASCII_SIZE>{});

    // Atualiza a nova matriz com os estados minimizados
    for (size_t i = 0; i < tamanho; ++i)
    {
        int novo_estado = estado_minimizado[i];
        if (novo_estado != -1)
        {
            for (size_t k = 0; k < ASCII_SIZE; ++k)
            {
                nova_matriz[novo_estado][k] = estado_minimizado[matriz[i][k]];
            }
        }
    }

    // Substitui a matriz original pela nova matriz minimizada
    this->matriz = std::move(nova_matriz);
    this->num_estados = estado_atual;

    // Remover estados antigos dos tokens
    for (size_t i = 0; i < tamanho; ++i)
    {
        // Verifica se o estado foi removido (não está mapeado no novo automato)
        if (estado_minimizado[i] != i)
        {
            // Remove o estado do TokenManager
            this->tokens.removeFinalState(i);
        }
    }

    // Atualiza a visualização
    this->printVisualizacaoDOT("afd-min.dot");
}

void AutomatoFinito::resolveEquivalencies(const size_t tamanho, std::vector<std::vector<bool>> &equivalencia)
{
    std::vector<std::vector<std::stack<std::pair<int, int>>>> dependencia(tamanho, std::vector<std::stack<std::pair<int, int>>>(tamanho));

    // Função lambda nomeada para processar a equivalência e dependências
    std::function<void(size_t, size_t)> processarDependencia;
    processarDependencia = [&](size_t i, size_t j)
    {
        // Não são equivalentes
        equivalencia[i][j] = false;
        equivalencia[j][i] = false;

        // Todo mundo que dependia também não é
        while (!dependencia[i][j].empty())
        {
            auto [estado_i, estado_j] = dependencia[i][j].top();
            dependencia[i][j].pop();
            processarDependencia(estado_i, estado_j);
        }
        while (!dependencia[j][i].empty())
        {
            auto [estado_i, estado_j] = dependencia[j][i].top();
            dependencia[j][i].pop();
            processarDependencia(estado_i, estado_j);
        }
    };

    // Percorrer a parte triangular inferior da matriz e verificar as transições
    for (size_t i = 1; i < tamanho; ++i)
    {
        for (size_t j = 0; j < i; ++j)
        {
            if (equivalencia[i][j])
            {
                // Para cada transição nos estados
                for (size_t k = 0; k < ASCII_SIZE; k++)
                {
                    size_t destino_i = matriz[i][k];
                    size_t destino_j = matriz[j][k];

                    // Se são diferentes então tem que verificar
                    if (destino_i != destino_j)
                    {
                        // Verifica se já foi percorrido
                        if (i > destino_i || (i == destino_i && j > destino_j))
                        {
                            // Foi percorrido
                            if (!equivalencia[destino_i][destino_j])
                            {
                                // Não são equivalentes e todo mundo que dependia também não é
                                processarDependencia(i, j);
                                break;
                            }
                        }
                        else
                        {
                            // Não foi percorrido
                            if (equivalencia[destino_i][destino_j])
                            {
                                // Não foi percorrido e está marcado como equivalente
                                dependencia[destino_i][destino_j].push({i, j});
                            }
                            else
                            {
                                // Não foi percorrido, mas não é equivalente, então eles não são equivalentes
                                // Todo mundo que dependia também não é
                                processarDependencia(i, j);
                                break;
                            }
                        }
                    }
                }
            }
        }
    }
}

void AutomatoFinito::toAFD()
{
    // Passo 1: Converte o AFND-e em um AFND
    this->printVisualizacaoDOT("afnd-e.dot");
    this->toAFND();
    this->printVisualizacaoDOT("afnd.dot");

    // Passo 2: Define as variáveis essenciais
    std::map<std::string, State *> mapa_assinaturas;
    std::stack<std::string> estados_restantes;
    auto getEstadosAssinatura = [](const std::string &assinatura) -> std::set<int>
    {
        std::set<int> estados;
        std::stringstream ss(assinatura);
        std::string item;

        while (std::getline(ss, item, ','))
        {
            estados.insert(std::stoi(item));
        }

        return estados;
    };

    // Passo 3: Realiza a conversão criando um novo autômato (AFD)
    GenericAutomata *afd = new GenericAutomata();

    // Inicializa com o estado inicial
    State *inicial = afd->createNewState();
    mapa_assinaturas[std::to_string(inicial->getEstado())] = inicial;
    estados_restantes.push(std::to_string(inicial->getEstado()));

    // Estrutura temporária para armazenar novos estados finais
    std::unordered_map<int, std::unordered_set<int>> novos_final_state_map;

    while (!estados_restantes.empty())
    {
        // Obtém a assinatura e os estados destino
        std::string assinatura = estados_restantes.top();
        estados_restantes.pop();
        State *atual = mapa_assinaturas[assinatura];
        std::set<int> estados_assinatura = getEstadosAssinatura(assinatura);

        // Mapa para agrupar transições por símbolo
        std::map<char, std::set<int>> transicoes_por_simbolo;

        // Percorre todos os estados representados pela assinatura atual
        for (int estado : estados_assinatura)
        {
            State *estado_antigo = afnd->findState(estado);

            // Se o estado antigo na assinatura for um estado final
            if (tokens.isFinalState(estado_antigo->getEstado()))
            {
                // Adiciona o estado atual ao novo final_state_map
                int token_id = this->tokens.getTokenIdByFinalState(estado_antigo->getEstado());
                novos_final_state_map[token_id].insert(atual->getEstado());
            }

            for (const auto &transicao : estado_antigo->getTransitions())
            {
                transicoes_por_simbolo[transicao.entrada].insert(transicao.estado_destino->getEstado());
            }
        }

        // Para cada símbolo, cria novas transições no AFD
        for (const auto &[simbolo, destinos] : transicoes_por_simbolo)
        {
            // Cria uma nova assinatura para o conjunto de destinos
            std::string nova_assinatura;
            for (int destino : destinos)
            {
                if (!nova_assinatura.empty())
                {
                    nova_assinatura += ",";
                }
                nova_assinatura += std::to_string(destino);
            }

            // Se a nova assinatura não existe no mapa, cria um novo estado
            if (mapa_assinaturas.find(nova_assinatura) == mapa_assinaturas.end())
            {
                State *novo_estado = afd->createNewState();
                mapa_assinaturas[nova_assinatura] = novo_estado;
                estados_restantes.push(nova_assinatura);
            }

            // Adiciona a transição no estado atual
            State *estado_destino = mapa_assinaturas[nova_assinatura];
            atual->addTransition(simbolo, estado_destino);
        }
    }

    // Remove estados inúteis do novo AFD
    afd->removeInutileStates();

    // Substitui o autômato antigo pelo novo
    delete this->afnd;
    this->afnd = afd;

    // Passo 4: Resolve duplicações no final_state_map garantindo que apenas o estado final com maior prioridade seja mantido
    for (auto &[token_id, estados] : novos_final_state_map)
    {
        if (estados.size() > 1)
        {
            // Encontrar o estado com menor valor numérico (maior prioridade)
            int priority_id = *std::min_element(estados.begin(), estados.end());
            
            // Limpar e inserir apenas o estado prioritário
            estados.clear();
            estados.insert(priority_id);
        }
    }

    // Passo 5: Atualiza o final_state_map do TokenManager existente
    this->tokens.replaceFinalStateMap(std::move(novos_final_state_map));

    this->printVisualizacaoDOT("afd.dot");

    // Passo 6: Transpõe para a estrutura do AFD
    this->transposeAFD();
}

void AutomatoFinito::toAFND()
{
    std::list<State *> estados = this->afnd->toList();
    GenericAutomata *novo_afnd = new GenericAutomata();

    // Para cada estado resolve suas transições lambdas gerando um novo AFND
    for (State *estado_antigo : estados)
    {
        State *novo_estado = novo_afnd->createNewState(estado_antigo->getEstado());

        // Obtém as novas transições sem as transições lambda
        std::vector<std::tuple<int, char, int>> novasTransicoes = resolveLambdaTransitions(estado_antigo);

        // Adiciona as transições ao novo estado
        for (const auto &[origem, entrada, destino] : novasTransicoes)
        {
            State *estado_destino = novo_afnd->findState(destino);
            if (estado_destino == nullptr)
            {
                // Cria o estado de destino no novo AFND se não existir
                estado_destino = novo_afnd->createNewState(destino);
            }
            novo_estado->addTransition(entrada, estado_destino);
        }
    }

    // Remove os estados inúteis
    novo_afnd->removeInutileStates();

    // Substitui o autômato antigo pelo novo
    delete afnd;
    this->afnd = novo_afnd;
}

std::vector<std::tuple<int, char, int>> AutomatoFinito::resolveLambdaTransitions(State *estado)
{
    std::vector<std::tuple<int, char, int>> novasTransicoes;

    std::function<void(State *, std::set<State *> &)> coletarTransicoes;
    coletarTransicoes = [&](State *estadoAtual, std::set<State *> &visitados)
    {
        if (visitados.find(estadoAtual) != visitados.end())
        {
            return; // Evita loops infinitos em casos de recursão
        }

        visitados.insert(estadoAtual);

        for (const Transition &transicao : estadoAtual->getTransitions())
        {
            if (transicao.entrada == '\0')
            { // Transição lambda
                coletarTransicoes(transicao.estado_destino, visitados);
                // Verifica se o estado de destino é final e obtém o token associado
                std::string token_name = this->tokens.getTokenByFinalState(transicao.estado_destino->getEstado());
                if (!token_name.empty())
                {
                    // Se o estado de destino for final, associe o mesmo token ao estado atual
                    this->tokens.setFinalState(token_name, estadoAtual->getEstado());
                }
            }
            else
            { // Transição terminal
                novasTransicoes.emplace_back(estado->getEstado(), transicao.entrada, transicao.estado_destino->getEstado());
            }
        }
    };

    std::set<State *> visitados;
    coletarTransicoes(estado, visitados);

    return novasTransicoes;
}

void AutomatoFinito::transposeAFD()
{
    this->num_estados = this->afnd->getNumEstados() + 1; // Adiciona o estado de erro
    std::list<State *> estados = this->afnd->toList();

    // Redimensiona a matriz para o novo número de estados
    matriz.resize(num_estados);
    for (auto &linha : matriz)
    {
        linha.fill(0); // Inicializa todas as posições da linha com 0
    }

    // Preenche a matriz com as transições existentes
    for (State *estado : estados)
    {
        int origem = estado->getEstado();
        for (const Transition &transicao : estado->getTransitions())
        {
            int input = static_cast<int>(transicao.entrada);
            int destino = transicao.estado_destino->getEstado();
            matriz[origem][input] = destino;
        }
    }

    // Remove o AFD original já que o AFD foi transposto
    delete afnd;
    afnd = nullptr;
    deterministico = true;
}

int AutomatoFinito::makeTransition(const int estado_atual, const char letra)
{
    if (!this->deterministico)
    {
        throw std::logic_error("O Autômato não é determinístico");
    }
    if (estado_atual >= num_estados || estado_atual < 0)
    {
        throw std::length_error("Estado Inválido");
    }
    int estado_destino = matriz[estado_atual][(int)letra];
    return estado_destino;
}

int AutomatoFinito::getNumEstados()
{
    return num_estados;
}

bool AutomatoFinito::setFinalState(const std::string &token, int final_state)
{
    if (final_state >= num_estados || final_state < 0)
    {
        throw std::length_error("Estado Inválido");
    }
    tokens.addToken(token);
    return tokens.setFinalState(token, final_state);
}

void AutomatoFinito::printTransitionTable(std::ostream &output)
{
    output << std::setw(15) << "Estado\\Input";
    for (int i = 0; i < ASCII_SIZE; ++i)
    {
        std::string rep;
        if (i < 32 || i == 127 || i >= 128)
        {
            // Para caracteres não visíveis ou com problemas na impressão
            rep = "ASCII " + std::to_string(i);
        }
        else
        {
            // Para caracteres visíveis no intervalo ASCII padrão (32 a 126)
            rep = std::string(1, static_cast<char>(i));
        }
        output << std::setw(15) << rep;
    }
    output << std::endl;
    if (deterministico)
    {
        // Printa a tabela do AFD
        for (int i = 0; i < num_estados; ++i)
        {
            output << std::setw(15) << "q" + std::to_string(i);
            for (int j = 0; j < ASCII_SIZE; ++j)
            {
                output << std::setw(15) << matriz[i][j];
            }
            output << std::endl;
        }
    }
    else
    {
        // Printa a tabela do AFND
        for (auto state : afnd->toList())
        {
            output << std::setw(15) << "q" + std::to_string(state->getEstado());
            for (int j = 0; j < ASCII_SIZE; ++j)
            {
                char input = static_cast<char>(j);
                bool transition_found = false;
                std::string trans_str = "{";

                // Itera sobre as transições para encontrar as correspondentes ao input
                for (const auto &transition : state->getTransitions())
                {
                    if (transition.entrada == input)
                    {
                        transition_found = true;
                        trans_str += "q" + std::to_string(transition.estado_destino->getEstado()) + ",";
                    }
                }

                if (transition_found)
                {
                    if (trans_str.back() == ',')
                    {
                        trans_str.pop_back(); // Remove a última vírgula
                    }
                    trans_str += "}";
                    output << std::setw(15) << trans_str;
                }
                else
                {
                    output << std::setw(15) << "-";
                }
            }
            output << std::endl;
        }
    }
}

void AutomatoFinito::printVisualizacaoDOT(const std::string &filename)
{
    std::ofstream out(filename);

    // Verifica se o arquivo foi aberto corretamente
    if (!out.is_open())
    {
        std::cerr << "Erro ao abrir o arquivo para escrita." << std::endl;
        return;
    }

    // Início do arquivo DOT
    out << "digraph AF {" << std::endl;
    out << "\trankdir=LR;" << std::endl; // Alinha os estados da esquerda para a direita
    out << "\tnode [shape=circle];" << std::endl;

    // Mapa de estados finais por token
    auto finalStatesByToken = this->tokens.getAllTokens();
    std::set<int> finalStates; // Conjunto de todos os estados finais

    // Adiciona os estados finais ao conjunto e os formata
    for (const auto &tokenPair : finalStatesByToken)
    {
        const std::string &tokenName = tokenPair.first;
        const std::vector<int> &states = tokenPair.second;

        for (int state : states)
        {
            finalStates.insert(state);
            out << "subgraph cluster" << state << " {" << std::endl
                << "\t\tlabelloc=\"b\";" << std::endl
                << "\t\tlabel=\"" << tokenName << "\";" << std::endl
                << "\t\tcolor=transparent;" << std::endl
                << "\t\tq" << state
                << " [shape=doublecircle, label=\"q" << state << "\"];}" << std::endl;
        }
    }

    if (this->deterministico)
    {
        // Geração do arquivo DOT para o AFD
        for (int i = 0; i < num_estados; ++i)
        {
            std::map<int, std::string> transicoesAgrupadas;

            for (int j = 0; j < ASCII_SIZE; ++j)
            {
                if (matriz[i][j] != 0)
                {
                    int destino = matriz[i][j];
                    char input = static_cast<char>(j);

                    // Adiciona o input ao mapa, agrupando por destino
                    if (transicoesAgrupadas.find(destino) != transicoesAgrupadas.end())
                    {
                        transicoesAgrupadas[destino] += input;
                    }
                    else
                    {
                        transicoesAgrupadas[destino] = std::string(1, input);
                    }
                }
            }

            // Para cada destino, processa a string de inputs para compactar sequências
            for (const auto &pair : transicoesAgrupadas)
            {
                int destino = pair.first;
                std::string inputs = pair.second;
                std::string label;

                // Ordena os inputs
                std::sort(inputs.begin(), inputs.end());

                // Compacta as sequências
                for (size_t k = 0; k < inputs.size(); ++k)
                {
                    char inicio = inputs[k];
                    char fim = inicio;

                    // Enquanto o próximo caractere formar uma sequência, continue
                    while (k + 1 < inputs.size() && inputs[k + 1] == fim + 1)
                    {
                        fim = inputs[++k];
                    }

                    if (inicio == fim)
                    {
                        label += inicio;
                    }
                    else
                    {
                        label += inicio + std::string("-") + fim;
                    }
                }

                out << "\tq" << i
                    << " -> q" << destino
                    << " [label=\"" << ((label == "\"") ? "\\\"" : label)  << "\"];" << std::endl;
            }
        }
    }
    else
    {
        // Geração do arquivo DOT para o AFND-e
        std::list<State *> estados = this->afnd->toList();

        for (State *estado : estados)
        {
            std::map<State *, std::string> transicoesAgrupadas;

            for (const auto &transicao : estado->getTransitions())
            {
                char input = transicao.entrada;
                State *destino = transicao.estado_destino;

                if (input != '\0')
                {
                    // Adiciona o input ao mapa, agrupando por destino
                    if (transicoesAgrupadas.find(destino) != transicoesAgrupadas.end())
                    {
                        transicoesAgrupadas[destino] += input;
                    }
                    else
                    {
                        transicoesAgrupadas[destino] = std::string(1, input);
                    }
                }
                else
                {
                    out << "\tq" << estado->getEstado()
                        << " -> q" << destino->getEstado()
                        << " [label=\"" << "\u03BB" << "\"];" << std::endl;
                }
            }

            // Para cada destino, processa a string de inputs para compactar sequências
            for (const auto &pair : transicoesAgrupadas)
            {
                State *destino = pair.first;
                std::string inputs = pair.second;
                std::string label;

                // Ordena os inputs
                std::sort(inputs.begin(), inputs.end());

                // Compacta as sequências
                for (size_t k = 0; k < inputs.size(); ++k)
                {
                    char inicio = inputs[k];
                    char fim = inicio;

                    // Enquanto o próximo caractere formar uma sequência, continue
                    while (k + 1 < inputs.size() && inputs[k + 1] == fim + 1)
                    {
                        fim = inputs[++k];
                    }

                    if (inicio == fim)
                    {
                        label += inicio;
                    }
                    else
                    {
                        label += inicio + std::string("-") + fim;
                    }
                }

                out << "\tq" << estado->getEstado()
                    << " -> q" << destino->getEstado()
                    << " [label=\"" << ((label == "\"") ? "\\\"" : label) << "\"];" << std::endl;
            }
        }
    }

    // Fim do arquivo DOT
    out << "}" << std::endl;
    out.close();
}

bool AutomatoFinito::operator==(const AutomatoFinito &outro) const
{
    if (num_estados != outro.num_estados)
    {
        return false;
    }
    if (!deterministico || !outro.deterministico)
    {
        // Só compara se os 2 forem determinísticos
        return false;
    }
    for (int i = 0; i < num_estados; ++i)
    {
        for (int j = 0; j < ASCII_SIZE; ++j)
        {
            if (matriz[i][j] != outro.matriz[i][j])
            {
                return false;
            }
        }
    }
    return true;
}
