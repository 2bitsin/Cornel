
BUILD_DIR 			?= $(abspath ../build)
BUILD_TARGETS 	?= stage1

.PHONY: all $(BUILD_TARGETS)

all: $(BUILD_TARGETS)

$(BUILD_TARGETS):
	for target in $(BUILD_TARGETS); do \
		make -C $$target BUILD_DIR=$(BUILD_DIR)/$$target TARGET_NAME=$$target all; \
	done





