# Definindo variáveis
CC = g++
CFLAGS = -g -std=c++11
INCLUDES = -I./include
SRC = $(wildcard src/*.cpp)
OBJ = $(SRC:src/%.cpp=obj/%.o)
EXEC = bin/tp2.out

# Compilação do projeto
all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) $(OBJ) -o $(EXEC)

# Regras para compilar os arquivos .cpp em .o
obj/%.o: src/%.cpp
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Limpar os arquivos compilados
clean:
	rm -f $(OBJ) $(EXEC)

# Regra para excluir os arquivos de objeto e o executável
mrproper: clean
	rm -f bin/*

# Executar o programa com um arquivo fornecido como argumento
run: $(EXEC)
	./$(EXEC) $(file)