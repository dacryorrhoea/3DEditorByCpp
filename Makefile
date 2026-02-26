PKG_CFLAGS := $(shell pkg-config --cflags sdl2 SDL2_ttf)
PKG_LIBS   := $(shell pkg-config --libs sdl2 SDL2_ttf)

SRC := $(wildcard src/*.cpp)
BUILD_DIR := build
INCLUDES := -I include
OBJ := $(patsubst src/%.cpp,$(BUILD_DIR)/%.o,$(SRC))
TARGET := app

.PHONY: all build run debug clean

all: build

build: CXXFLAGS := -std=c++23 -O2 -Wall -Wextra $(PKG_CFLAGS) $(INCLUDES)
build: $(TARGET)

debug: CXXFLAGS := -std=c++23 -O0 -g -Wall -Wextra $(PKG_CFLAGS) $(INCLUDES)
debug: $(TARGET)
	@gdb -tui ./$(TARGET)

$(TARGET): $(OBJ)
	@g++ $(CXXFLAGS) -o $@ $^ $(PKG_LIBS)

$(BUILD_DIR)/%.o: src/%.cpp
	@g++ $(CXXFLAGS) -c $< -o $@

run: build
	@./$(TARGET)

clean:
	@rm -f $(OBJ) $(TARGET)