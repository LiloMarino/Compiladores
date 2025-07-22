# Nome do executável
EXEC_NAME = exame

# Nome do zip
ZIP_NAME = exame

# Detectar automaticamente os arquivos fonte e headers
SOURCES = $(wildcard *.cpp)
HEADERS = $(wildcard *.hpp)

# Lista de arquivos utilizados
FILES = $(SOURCES) $(HEADERS) Makefile

# Compilador
CXX = g++

# Flags de compilação
CXXFLAGS = -Wall -Wextra -std=c++17 -g

# Arquivos objeto
OBJECTS = $(SOURCES:.cpp=.o)

# Alvo padrão
all: $(EXEC_NAME)

# Compilação final
$(EXEC_NAME): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJECTS)

# Compilação dos objetos
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $<

# Inclusão de dependências (opcional)
-include $(OBJECTS:.o=.d)

# Regra para rodar
run: all $(EXEC_NAME)
	./$(EXEC_NAME)

# Compactar os arquivos
zip: $(ZIP_NAME).zip

$(ZIP_NAME).zip: $(FILES)
	zip -r $(ZIP_NAME).zip $(FILES)
	mkdir -p $(ZIP_NAME)
	unzip -q $(ZIP_NAME).zip -d $(ZIP_NAME)

# Limpar os arquivos gerados
clean:
	rm -f $(OBJECTS)
	rm -f $(SOURCES:.cpp=.d)
	rm -f $(EXEC_NAME)

# Executar com Valgrind
valgrind: all
	valgrind --leak-check=full --show-leak-kinds=all ./$(EXEC_NAME)

finish: all run clean zip

.PHONY: all clean run zip valgrind finish
