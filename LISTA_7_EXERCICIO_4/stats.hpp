#ifndef STATS_HPP
#define STATS_HPP

#include <string>

struct Stats
{
    int total_lines;
    int blank_lines;
    int lines_with_comments;
    int nb_nc_lines;
    int semi_colons;
    int prepoc_direct;
    std::string file_name;
};

extern Stats atual;

#endif