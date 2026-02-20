SRC := $(wildcard *.cpp)
OBJ := $(SRC:.cpp=.o)
TARGET := app

.PHONY: all build run debug clean

all: build

build: CXXFLAGS := -std=c++23 -O2 -Wall -Wextra
build: $(TARGET)

debug: CXXFLAGS := -std=c++23 -O0 -g -Wall -Wextra
debug: clean $(TARGET)
	@gdb -tui ./$(TARGET)

$(TARGET): $(OBJ)
	@g++ $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	@g++ $(CXXFLAGS) -c $< -o $@

run: build
	@rm -f $(OBJ)
	@./$(TARGET)

clean:
	@rm -f $(OBJ) $(TARGET)
