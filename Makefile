# 排除main.c main.cpp
CSRC = $(filter-out main.c, $(wildcard *.c))
CXXSRC = $(filter-out main.cpp, $(wildcard *.cpp))

OBJ = $(patsubst %.c, $(BUILD_DIR)/%.o, $(CSRC)) $(patsubst %.cpp, $(BUILD_DIR)/%.o, $(CXXSRC))

CC = gcc
CXX = g++

CFLAGS = -g

CXXFLAGS = -std=c++17 -g

TARGET = main

BUILD_DIR = build

RESULTS_DIR = results

TARGET = main

default: $(BUILD_DIR)/$(TARGET)

$(BUILD_DIR)/main.o: 
	@mkdir -p $(BUILD_DIR)
	@if [ "$(GRAMMAR)" = "LR1" ]; then \
		$(CXX) $(CXXFLAGS) -c -o $(BUILD_DIR)/main.o main.cpp; \
	else \
		$(CC) $(CFLAGS) -c -o $(BUILD_DIR)/main.o main.c; \
	fi

$(BUILD_DIR)/%.o: %.c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c -o $@ $<

$(BUILD_DIR)/%.o: %.cpp
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(BUILD_DIR)/$(TARGET): $(OBJ) $(BUILD_DIR)/main.o
	@mkdir -p $(BUILD_DIR)
	@mkdir -p $(RESULTS_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $^

image:
	dot -Tpng $(RESULTS_DIR)/analysisTree.dot -o $(RESULTS_DIR)/analysisTree.png

gdb: $(BUILD_DIR)/$(TARGET)
	gdb $(BUILD_DIR)/$(TARGET) -- $(ARGS)

lldb: $(BUILD_DIR)/$(TARGET)
	lldb $(BUILD_DIR)/$(TARGET) -- $(ARGS)

.PHONY: clean

clean:
	rm -rf $(BUILD_DIR)
	