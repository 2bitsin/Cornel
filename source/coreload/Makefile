include .config

TARGET_NAME			?= $(shell basename $(abspath .))
TARGET_BINARY		?= $(BUILD_DIR)/$(TARGET_NAME).sys
BUILD_DIR 			?= $(abspath ../../build/$(TARGET_NAME))

BUILD_TARGETS 	?= stub loader
BUILD_BINARIES	?= $(addprefix $(BUILD_DIR)/,$(addsuffix .bin,$(BUILD_TARGETS)))

.PHONY: all
all: $(BUILD_DIR) $(TARGET_BINARY)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)/*

$(BUILD_BINARIES):
	for target in $(BUILD_TARGETS); do \
		$(MAKE) -C $$target BUILD_DIR=$(BUILD_DIR)/$$target all; \
		cp $(BUILD_DIR)/$$target/$$target.bin $(BUILD_DIR)/; \
	done

$(TARGET_BINARY): $(BUILD_BINARIES)
	cat $(BUILD_BINARIES) >$(TARGET_BINARY)

	








