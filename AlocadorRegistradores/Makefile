# Nome do executável
EXEC_NAME = regalloc

# Nome do zip
ZIP_NAME = regalloc

# Detectar automaticamente os arquivos fonte
SOURCES = $(wildcard *.cpp)
LEXER = lex.yy.c
PARSER = syntax.tab.c syntax.tab.h

# Detectar automaticamente os headers correspondentes
HEADERS = $(wildcard *.hpp)

# Lista todos os arquivos envolvidos
FILES = $(SOURCES) $(HEADERS) Makefile lex.l syntax.y

# Definir o compilador
CXX = g++
LEX = flex
YACC = bison

# Definir flags do compilador
CXXFLAGS = -Wall -Wextra -std=c++17 -g

# Gerar a lista de arquivos objeto a partir dos arquivos fonte
OBJECTS = $(SOURCES:.cpp=.o) lex.yy.o syntax.tab.o

# Alvo padrão
all: $(LEXER) $(PARSER) $(EXEC_NAME)

# Regras de compilação
$(EXEC_NAME): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJECTS)

syntax.tab.c syntax.tab.h: syntax.y
	$(YACC) -d syntax.y

$(LEXER): lex.l
	$(LEX) lex.l

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $<

syntax.tab.o: syntax.tab.c
	$(CXX) $(CXXFLAGS) -c $<

lex.yy.o: $(LEXER)
	$(CXX) $(CXXFLAGS) -c $(LEXER)

-include $(OBJECTS:.o=.d)

# run: Compila e roda o programa
run: all $(EXEC_NAME)
	./$(EXEC_NAME)

# zip: Zippa os arquivos fonte
zip: $(ZIP_NAME).zip

$(ZIP_NAME).zip: $(FILES)
	zip -r $(ZIP_NAME).zip $(FILES)
	mkdir -p $(ZIP_NAME)
	unzip -q $(ZIP_NAME).zip -d $(ZIP_NAME)

# clean: Limpa todos os arquivos gerados da compilação
clean:
	rm -f $(OBJECTS)
	rm -f $(SOURCES:.cpp=.d)
	rm -f $(EXEC_NAME)
	rm -f $(LEXER) syntax.tab.c syntax.tab.h
	rm -f *.o

# valgrind: Regra para executar o programa com o Valgrind
valgrind: all
	valgrind --leak-check=full --show-leak-kinds=all ./$(EXEC_NAME)

finish: all run clean zip

.PHONY: all clean run zip
