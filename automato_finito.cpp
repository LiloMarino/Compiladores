#include "automato_finito.hpp"
#include <string>
#include <stdexcept>
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <fstream>
#include <map>

#define ASCII_SIZE 256

AutomatoFinito::AutomatoFinito() : num_estados(0), matriz(nullptr), deterministico(false), afnd(nullptr)
{
    afnd = new GenericAutomata();
}

AutomatoFinito::AutomatoFinito(int num_estados) : matriz(nullptr), afnd(nullptr)
{
    num_estados++; // Adiciona o estado de erro
    this->num_estados = num_estados;
    matriz = new int *[num_estados]();
    for (int i = 0; i < num_estados; ++i)
    {
        matriz[i] = new int[ASCII_SIZE]();
    }
    deterministico = true;
}

AutomatoFinito::~AutomatoFinito()
{
    if (matriz != nullptr)
    {
        for (int i = 0; i < num_estados; ++i)
        {
            delete[] matriz[i];
        }
        delete[] matriz;
    }
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

void AutomatoFinito::addRegularExpression(const std::string &re)
{
    if (this->deterministico)
    {
        throw std::logic_error("O Autômato é determinístico");
    }
    this->afnd->addRegularExpression(re);
}

void AutomatoFinito::toAFD()
{
    // Passo 1: Converte o AFND-e em um AFND
    this->toAFND();
    this->printVisualizacaoDOT("afnd.dot");
}

void AutomatoFinito::toAFND() {
    // Obtém a lista de estados do AFND-e atual
    std::list<State *> estados = this->afnd->toList();
    
    // Cria um novo AFND sem transições lambda
    GenericAutomata *novo_afnd = new GenericAutomata();

    // Itera pelos estados do autômato antigo
    for (State* estado_antigo : estados) {
        // Cria um novo estado correspondente no novo AFND
        State* novo_estado = novo_afnd->createNewState();
        novo_estado->setEstado(estado_antigo->getEstado()); // Mantém o número do estado

        // Elimina as transições lambda e obtém as novas transições
        std::vector<std::tuple<int, char, int>> novasTransicoes = resolveLambdaTransitions(estado_antigo);

        // Adiciona as novas transições ao novo estado
        for (const auto& [origem, entrada, destino] : novasTransicoes) {
            State* estado_destino = novo_afnd->findState(destino);
            if (estado_destino == nullptr) {
                // Cria o estado de destino no novo AFND se não existir
                estado_destino = novo_afnd->createNewState();
                estado_destino->setEstado(destino);
            }
            novo_estado->addTransition(entrada, estado_destino);
        }
    }

    // Substitui o autômato antigo pelo novo
    delete afnd;
    this->afnd = novo_afnd;
}

std::vector<std::tuple<int, char, int>> AutomatoFinito::resolveLambdaTransitions(State* estado) {
    std::vector<std::tuple<int, char, int>> novasTransicoes;

    std::function<void(State*, std::set<State*>&)> coletarTransicoes;
    coletarTransicoes = [&](State* estadoAtual, std::set<State*>& visitados) {
        if (visitados.find(estadoAtual) != visitados.end()) {
            return; // Evita loops infinitos em casos de recursão
        }

        visitados.insert(estadoAtual);

        for (const Transition& transicao : estadoAtual->getTransitions()) {
            if (transicao.entrada == '\0') { // Transição lambda
                coletarTransicoes(transicao.estado_destino, visitados);
            } else { // Transição terminal
                novasTransicoes.emplace_back(estado->getEstado(), transicao.entrada, transicao.estado_destino->getEstado());
            }
        }
    };

    std::set<State*> visitados;
    coletarTransicoes(estado, visitados);

    return novasTransicoes;
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
        for (auto &state : *afnd)
        {
            output << std::setw(15) << "q" + std::to_string(state.getEstado());
            for (int j = 0; j < ASCII_SIZE; ++j)
            {
                char input = static_cast<char>(j);
                bool transition_found = false;
                std::string trans_str = "{";

                // Itera sobre as transições para encontrar as correspondentes ao input
                for (const auto &transition : state.getTransitions())
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

    if (this->deterministico)
    {
        // Geração do arquivo DOT para o AFD
        for (int i = 0; i < num_estados; ++i)
        {
            for (int j = 0; j < ASCII_SIZE; ++j)
            {
                if (matriz[i][j] != 0)
                {
                    out << "\tq" << std::to_string(i)
                        << " -> q" << matriz[i][j]
                        << " [label=\"" << static_cast<char>(j) << "\"];" << std::endl;
                }
            }
        }
    }
    else
    {
        // Geração do arquivo DOT para o AFND-e
        std::list<State *> estados = this->afnd->toList();

        for (State *estado : estados)
        {
            for (const auto &transicao : estado->getTransitions())
            {
                if (transicao.entrada != '\0')
                {
                    out << "\tq" << estado->getEstado()
                        << " -> q" << transicao.estado_destino->getEstado()
                        << " [label=\"" << transicao.entrada << "\"];" << std::endl;
                }
                else
                {
                    out << "\tq" << estado->getEstado()
                        << " -> q" << transicao.estado_destino->getEstado()
                        << " [label=\"" << "\u03BB" << "\"];" << std::endl;
                }
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
