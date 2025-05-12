#include <iostream>
#include <iomanip>
#include <vector>
#include <fstream>
#include "lexer.hpp"
#include "stats.hpp"

Stats atual;

void montarTabela(std::vector<Stats> &arquivos)
{
    // Header
    std::cout << std::setw(7) << "total"
              << std::setw(9) << "blank"
              << std::setw(9) << "lines w/"
              << std::setw(9) << "nb, nc"
              << std::setw(9) << "semi-"
              << std::setw(9) << "preproc."
              << std::setw(15) << "file" << std::endl;

    std::cout << std::setw(7) << "lines"
              << std::setw(9) << "lines"
              << std::setw(9) << "comments"
              << std::setw(9) << "lines"
              << std::setw(9) << "colons"
              << std::setw(9) << "direct."
              << std::setw(15) << "" << std::endl;

    std::cout << "------+--------+--------+--------+--------+--------+----------------" << std::endl;

    // Table
    for (const auto &arquivo : arquivos)
    {
        std::cout << std::setw(7) << arquivo.total_lines
                  << std::setw(9) << arquivo.blank_lines
                  << std::setw(9) << arquivo.lines_with_comments
                  << std::setw(9) << arquivo.nb_nc_lines
                  << std::setw(9) << arquivo.semi_colons
                  << std::setw(9) << arquivo.prepoc_direct
                  << std::setw(15) << arquivo.file_name << std::endl;
    }
}

int main(int argc, char *argv[])
{
    // Se faltar argumentos então retorna um erro
    if (argc < 2)
    {
        std::cerr << "Uso: " << argv[0] << " <arquivo1> <arquivo2> ... <arquivoN>" << std::endl;
        return 1;
    }

    // Guarda todos os arquivos num vetor
    std::vector<Stats> arquivos;
    arquivos.reserve(argc - 1);
    for (int i = 1; i < argc; ++i)
    {
        Stats filestats = {0, 0, 0, 0, 0, 0, argv[i]};
        arquivos.push_back(filestats);
    }

    // Para cada arquivo no vetor obtém as estatísticas dele
    for (auto &arquivo : arquivos)
    {
        atual = arquivo;
        std::ifstream file(arquivo.file_name);
        if (!file)
        {
            std::cerr << "Erro ao abrir o arquivo: " << arquivo.file_name << std::endl;
            continue;
        }

        MyLexer lexer(file, std::cout);
        Token token;
        while ((token = lexer.nextToken()) != Token::END_OF_FILE) {}
        arquivo = atual;
    }

    // Cálcula o total
    Stats total = {0, 0, 0, 0, 0, 0, "total"};
    for (const auto &arquivo : arquivos)
    {
        total.total_lines += arquivo.total_lines;
        total.blank_lines += arquivo.blank_lines;
        total.lines_with_comments += arquivo.lines_with_comments;
        total.nb_nc_lines += arquivo.nb_nc_lines;
        total.semi_colons += arquivo.semi_colons;
        total.prepoc_direct += arquivo.prepoc_direct;
    }
    arquivos.push_back(total);

    montarTabela(arquivos);
    return 0;
}
