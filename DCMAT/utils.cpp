#include "utils.hpp"
#include "dcmat.hpp"
#include <iostream>
#include <iomanip>

void init()
{
    std::cout << std::fixed << std::setprecision(dcmat.settings.float_precision);
}

void yyerror(const char *s)
{
    std::cout << s << std::endl;
}