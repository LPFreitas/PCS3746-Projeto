# Variáveis para o compilador e opções de compilação
CXX = g++
CXXFLAGS = -lstdc++

# Nome do executável
EXECUTABLE = main

# Lista de arquivos fonte (main.cpp e outras classes)
SOURCES = main.cpp sistema_operacional.cpp fila_de_prontos.cpp processo_so.cpp processo_usuario.cpp

# Gera os nomes dos arquivos objeto substituindo a extensão .cpp por .o
OBJECTS = $(SOURCES:.cpp=.o) 

# Regra default (o que será executado ao chamar 'make' sem argumentos)
all: $(EXECUTABLE)

# Regra para gerar o executável
$(EXECUTABLE): $(OBJECTS) processo.hpp
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o $(EXECUTABLE)

# Regra para compilar os arquivos fonte em arquivos objeto
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

envia: envia.cpp
	$(CXX) $(CXXFLAGS) envia.cpp -o envia 

# Limpa os arquivos objeto e o executável
clean:
	rm -f $(OBJECTS) $(EXECUTABLE)
