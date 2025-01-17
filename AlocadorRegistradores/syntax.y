%code requires {
// Necessário para evitar problemas de inclusão
// Coloca na .h
#include <list>
#include <utility>
#include "graph.hpp"
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
}

%token <integer> INTEGER

%token GRAFO ARROW COLON ASSIGN K

%type <list_int> IntegerLoop
%type <pair_int_list> GraphNode
%type <graph> GraphNodes

%start Grafo

%%

Grafo: GRAFO INTEGER COLON GraphData {
        
      }
     ;

GraphData: K ASSIGN INTEGER GraphNodes {
            
          }
         ;

GraphNodes: GraphNode GraphNodes {
              
            }
          | GraphNode {
              
            }
          ;

GraphNode: INTEGER ARROW IntegerLoop {
            $$ = new std::pair<int, std::list<int>*>($1, $3);
          }
         ;

IntegerLoop: INTEGER IntegerLoop { 
              $$ = new std::list<int>(*$2);
              $$->push_front($1);
              delete $2;  
            }
           | INTEGER { 
              $$ = new std::list<int>();
              $$->push_front($1); 
            } 
           ;

%%

void yyerror(const char *) {
}