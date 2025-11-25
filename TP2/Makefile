# Compilador e flags
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17 -Iinclude

# Diretórios
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin

# Nome do executável final
TARGET = $(BIN_DIR)/tp2.out

# Lista de arquivos fonte
SRCS = $(SRC_DIR)/demanda.cpp \
       $(SRC_DIR)/parada.cpp \
       $(SRC_DIR)/trecho.cpp \
       $(SRC_DIR)/corrida.cpp \
       $(SRC_DIR)/escalonador.cpp \
       $(SRC_DIR)/minheap.cpp \
       $(SRC_DIR)/main.cpp

# Objetos correspondentes
OBJS = $(SRCS:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

# Regra padrão
all: $(TARGET)

# Linkar o executável final
$(TARGET): $(OBJS)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Compilar cada .cpp em .o dentro de obj/
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Limpar objetos e binários
clean:
	rm -rf $(OBJ_DIR)/*.o $(TARGET)

# Limpeza total
fclean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

# Recompilar tudo
re: clean all

.PHONY: all clean re fclean
