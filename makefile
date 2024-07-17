# Nome do executável
TARGET = compilador

# Lista de arquivos fonte
SOURCES = main.cpp automato.cpp reconhecedor.cpp

# Gerar lista de headers automaticamente (qualquer .cpp que não seja main.cpp)
HEADERS = $(patsubst %.cpp,%.hpp,$(filter-out main.cpp,$(SOURCES)))

# Definir o compilador
CXX = g++

# Definir flags do compilador
CXXFLAGS = -Wall -Wextra -std=c++17 -g

# Gerar a lista de arquivos objeto a partir dos arquivos fonte
OBJECTS = $(SOURCES:.cpp=.o)

# Regra padrão para compilar o projeto
all: $(TARGET)

# Regra para gerar o executável
$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Regra para compilar arquivos .cpp em .o e gerar dependências
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -MMD -c $< -o $@

# Incluir dependências geradas automaticamente
-include $(SOURCES:.cpp=.d)

# Função run: compilar e rodar o executável
run: $(TARGET)
	$(TARGET)

# Função zip: criar um arquivo zip com fontes e headers
zip:
	tar -cf fontes.zip $(SOURCES) $(HEADERS) makefile

# Limpeza dos arquivos gerados
clean:
	del $(subst /,\,$(OBJECTS)) $(subst /,\,$(SOURCES:.cpp=.d))

# Limpeza geral
clean_all: clean
	del $(subst /,\,$(TARGET).exe)

finish: all run clean_all zip

.PHONY: all clean run zip
