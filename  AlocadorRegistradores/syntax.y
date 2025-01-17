%code requires {
// Necessário para evitar problemas de inclusão
// Coloca na .h
}

%{
// Coloca na .c
extern int yylex();
void yyerror(const char *msg);
%}

%union {
    int integer;
}

%token <integer> INTEGER

%token GRAFO ARROW COLON ASSIGN K

%start Graph

%%

Graph: GRAFO INTEGER COLON GraphData {}
     ;

GraphData: K ASSIGN INTEGER GraphNodes {}
         ;

GraphNodes: GraphNode GraphNodes {}
          | GraphNode {}
          ;

GraphNode: INTEGER ARROW IntegerLoop {}
         ;

IntegerLoop: INTEGER IntegerLoop {}
           | INTEGER {} 
           ;

%%

void yyerror(const char *) {
}