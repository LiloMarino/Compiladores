%code requires {
// Necessário para evitar problemas de inclusão
// Coloca na .h
#include <list>
#include <utility>
#include "linearscan.hpp"
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
    LinearScan* linear_scan;
}

%token <integer> INTEGER

%token NEWLINE ARROW ASSIGN K VR

%type <list_int> IntegerLoop
%type <pair_int_list> LifeTime
%type <linear_scan> LifeTimes

%start Start

%%

Start: K ASSIGN INTEGER NEWLINE LifeTimes {
            $5->start($3);
          }
         ;

LifeTimes: LifeTime NEWLINE LifeTimes {
              $$ = $3;
              $$->addVirtualRegister($1->first, *($1->second));
              delete $1->second;
              delete $1;
            }
          | LifeTime {
              $$ = new LinearScan();
              $$->addVirtualRegister($1->first, *($1->second));
              delete $1->second;
              delete $1;
            }
          ;

LifeTime: VR INTEGER ARROW IntegerLoop {
            $$ = new std::pair<int, std::list<int>*>($2, $4);
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