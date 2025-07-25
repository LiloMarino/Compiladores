# Nome do executável
EXEC_NAME = l14e1

# Nome do zip
ZIP_NAME = LISTA_14_EXERCICIO_1

# Lista de arquivos fonte
SOURCES = main.cpp utils.cpp
LEXER = lex.yy.c
PARSER = parser.tab.c parser.tab.h

# Lista de headers automaticamente (qualquer .cpp que não seja main.cpp)
HEADERS = $(patsubst %.cpp,%.hpp,$(filter-out main.cpp,$(SOURCES)))

# Lista todos os arquivos envolvidos
FILES = $(SOURCES) $(HEADERS) makefile lexer.l parser.y

# Definir o compilador
CXX = g++
LEX = flex
YACC = bison

# Definir flags do compilador
CXXFLAGS = -Wall -Wextra -std=c++17 -g

# Gerar a lista de arquivos objeto a partir dos arquivos fonte
OBJECTS = $(SOURCES:.cpp=.o) lex.yy.o parser.tab.o

# Alvo padrão
all: $(LEXER) $(PARSER) $(EXEC_NAME)

# Regras de compilação
$(EXEC_NAME): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJECTS)

parser.tab.c parser.tab.h: parser.y
	$(YACC) -d parser.y

$(LEXER): lexer.l
	$(LEX) lexer.l

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $<

parser.tab.o: parser.tab.c
	$(CXX) $(CXXFLAGS) -c $<

lex.yy.o: $(LEXER)
	$(CXX) $(CXXFLAGS) -c $(LEXER)

-include $(OBJECTS:.o=.d)

# run: Compila e roda o programa
run: $(EXEC_NAME)
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
	rm -f $(LEXER) parser.tab.c parser.tab.h
	rm -f *.o

# valgrind: Regra para executar o programa com o Valgrind
valgrind: all
	valgrind --leak-check=full --show-leak-kinds=all ./$(EXEC_NAME)

finish: all run clean zip

.PHONY: all clean run zip
