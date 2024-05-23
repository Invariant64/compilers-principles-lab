# 排除main.c main.cpp
SRC_DIR = src

# 在SRC_DIR中的所有.c文件
CSRC = $(filter-out main.c, $(wildcard $(SRC_DIR)/*.c))
CXXSRC = $(filter-out main.cpp, $(wildcard $(SRC_DIR)/*.cpp))

OBJ = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(CSRC)) $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(CXXSRC))

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
		$(CXX) $(CXXFLAGS) -c -o $(BUILD_DIR)/main.o $(SRC_DIR)/main.cpp; \
	else \
		$(CC) $(CFLAGS) -c -o $(BUILD_DIR)/main.o $(SRC_DIR)/main.c; \
	fi

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c -o $@ $<

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
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
	