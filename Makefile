LOCAL_PATH = $(abspath .)

CSRC = $(shell find $(LOCAL_PATH) -name "*.c" -o -name "*.cpp" -maxdepth 1)

CC = g++

CFLAGS = -Wall -g

TARGET = main

BUILD_DIR = $(LOCAL_PATH)/build

default: $(BUILD_DIR)/$(TARGET)

image:
	dot -Tpng analysisTree.dot -o analysisTree.png

lldb: $(BUILD_DIR)/$(TARGET)
	lldb $(BUILD_DIR)/$(TARGET) -- $(ARGS)

$(BUILD_DIR)/$(TARGET): $(CSRC)
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -o $@ $^

.PHONY: clean

clean:
	rm -rf $(BUILD_DIR)
	