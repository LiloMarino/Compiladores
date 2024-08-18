#ifndef STATE_HPP
#define STATE_HPP

#include <unordered_map>

class State
{
private:
    int estado;
    std::unordered_map<char,State*> transitions;
public:
};

#endif
