CSRC = $(wildcard *.c)
CXXSRC += $(wildcard *.cpp)
OBJ = $(patsubst %.c, $(BUILD_DIR)/%.o, $(CSRC)) $(patsubst %.cpp, $(BUILD_DIR)/%.o, $(CXXSRC))

CC = gcc
CXX = g++

CXXFLAGS = -std=c++17

BUILD_DIR = build

TARGET = main

$(BUILD_DIR)/%.o: %.c
	$(CC) -c -o $@ $<

$(BUILD_DIR)/%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

default: $(BUILD_DIR)/$(TARGET)

$(BUILD_DIR)/$(TARGET): $(OBJ)
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $^

.PHONY: clean

clean:
	rm -rf $(BUILD_DIR)
