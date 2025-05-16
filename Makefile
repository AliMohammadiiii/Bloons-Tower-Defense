CXX ?= g++
SFML_INC ?= -I/opt/homebrew/include   
SFML_LIB ?= -L/opt/homebrew/lib

CXXFLAGS ?= -std=c++20 -Wall -Wextra -Iinclude $(SFML_INC)
LDFLAGS  ?= $(SFML_LIB) -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio
SRC_DIR := src
OBJ_DIR := build
BIN_DIR := bin

SRC := $(shell find $(SRC_DIR) -name '*.cpp')
OBJ := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC))

TARGET := $(BIN_DIR)/TD

.PHONY: all
all: $(TARGET)

$(TARGET): $(OBJ) | $(BIN_DIR)
	$(CXX) $(OBJ) -o $@ $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BIN_DIR):
	@mkdir -p $@

.PHONY: run clean debug release

run: all
	./$(TARGET) ./assets/maps/sample.map

debug: CXXFLAGS += -g -O0
debug: all

release: CXXFLAGS += -O3 -DNDEBUG
release: all

clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

