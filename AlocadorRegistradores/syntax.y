%code requires {
// Necessário para evitar problemas de inclusão
// Coloca na .h
#include <list>
#include <utility>
#include "graph.hpp"
#include "regalloc.hpp"
}

%{
// Coloca na .c
extern int yylex();
void yyerror(const char *msg);
%}

%union {
    int integer;
    std::list<int>*  list_int;
    std::pair<int, std::list<int>*>* pair_int_list;
    Graph* graph;
    RegAlloc* regalloc;
}

%token <integer> INTEGER

%token GRAFO ARROW COLON ASSIGN K

%type <list_int> IntegerLoop
%type <pair_int_list> GraphNode
%type <graph> GraphNodes
%type <regalloc> GraphData

%start Grafo

%%

Grafo: GRAFO INTEGER COLON GraphData {
        $4->start();
        delete $4;
      }
     ;

GraphData: K ASSIGN INTEGER GraphNodes {
            $$ = new RegAlloc(std::unique_ptr<Graph>($4), $3);
          }
         ;

GraphNodes: GraphNode GraphNodes {
              $$ = $2;
              $$->addAdjacencyList($1->first, *($1->second));
              delete $1->second;
              delete $1;
            }
          | GraphNode {
              $$ = new Graph();
              $$->addAdjacencyList($1->first, *($1->second));
              delete $1->second;
              delete $1;
            }
          ;

GraphNode: INTEGER ARROW IntegerLoop {
            $$ = new std::pair<int, std::list<int>*>($1, $3);
          }
         ;

IntegerLoop: INTEGER IntegerLoop { 
              $$ = $2;
              $$->push_front($1);
            }
           | INTEGER { 
              $$ = new std::list<int>();
              $$->push_front($1);
            } 
           ;

%%

void yyerror(const char *) {
}