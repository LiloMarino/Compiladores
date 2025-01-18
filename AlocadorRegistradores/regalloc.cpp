#include "regalloc.hpp"

RegAlloc::RegAlloc(std::unique_ptr<Graph> grafo, int K) 
    : grafo(std::move(grafo)), K(K) {}
