# Compilador e flags
CXX      = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -O2
# CXXFLAGS = -std=c++11 -g -Wall -Wextra   # use esta linha para depurar

# Pastas
INCLUDE_FOLDER = ./include
BIN_FOLDER     = ./bin
OBJ_FOLDER     = ./obj
SRC_FOLDER     = ./src

# Alvo final exigido
TARGET = tp1.out

# Fontes: aceita .cpp e .cc
SRC_CPP = $(wildcard $(SRC_FOLDER)/*.cpp)
SRC_CC  = $(wildcard $(SRC_FOLDER)/*.cc)
SRC     = $(SRC_CPP) $(SRC_CC)

# Objetos em obj/
OBJ = $(patsubst $(SRC_FOLDER)/%.cpp, $(OBJ_FOLDER)/%.o, \
      $(patsubst $(SRC_FOLDER)/%.cc,  $(OBJ_FOLDER)/%.o, $(SRC)))

# Regras genéricas de compilação
$(OBJ_FOLDER)/%.o: $(SRC_FOLDER)/%.cpp | dirs
	$(CXX) $(CXXFLAGS) -I$(INCLUDE_FOLDER) -c $< -o $@

$(OBJ_FOLDER)/%.o: $(SRC_FOLDER)/%.cc | dirs
	$(CXX) $(CXXFLAGS) -I$(INCLUDE_FOLDER) -c $< -o $@

# Garante que as pastas existem
.PHONY: dirs
dirs:
	@mkdir -p $(OBJ_FOLDER) $(BIN_FOLDER)

# Linkagem
all: dirs $(OBJ)
	$(CXX) $(CXXFLAGS) -o $(BIN_FOLDER)/$(TARGET) $(OBJ)

# Limpeza
.PHONY: clean
clean:
	@rm -f $(OBJ_FOLDER)/*.o $(BIN_FOLDER)/*
