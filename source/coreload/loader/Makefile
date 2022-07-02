include .config

TARGET_NAME					?=  $(shell basename $(abspath .))
BUILD_DIR						?=	$(abspath ../../../build/$(TARGET_NAME))
SOURCES_DIR					?= 	$(abspath .)
TARGET_BINARY_NAME 	?= 	$(TARGET_NAME).bin
TARGET_BINARY 			?= 	$(BUILD_DIR)/$(TARGET_BINARY_NAME)
MAP_FILE						?= 	$(TARGET_BINARY).map

.PHONY: all
all: $(BUILD_DIR) $(TARGET_BINARY) 

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)/*

$(TARGET_BINARY):
	echo "Hello World!">$(TARGET_BINARY)
