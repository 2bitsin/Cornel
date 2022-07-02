include .config

BUILD_DIR 				?= $(abspath ../build)
TARGET_NAME				?= cornel
TARGET_BINARY			?= $(BUILD_DIR)/$(TARGET_NAME).iso

BUILD_TARGETS			?= coreload.sys

PREFIXED_TARGETS	?= $(addprefix $(BUILD_DIR)/, $(BUILD_TARGETS))

.PHONY: all
all: $(BUILD_DIR) $(TARGET_BINARY)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)/*

$(PREFIXED_TARGETS):
	for target in $(BUILD_TARGETS); do \
		$(MAKE) -C "$${target%.*}" BUILD_DIR="$(BUILD_DIR)/$${target%.*}" all; \
		cp "$(BUILD_DIR)/$${target%.*}/$$target" $(BUILD_DIR)/$$target;	\
	done

$(TARGET_BINARY): $(PREFIXED_TARGETS)
	mkdir -p $(BUILD_DIR)/iso/cornel/
	cp $(PREFIXED_TARGETS) $(BUILD_DIR)/iso/cornel/
	mkisofs -input-charset utf8 -pad -R -J -c cornel/boot.cat -b cornel/coreload.sys -no-emul-boot -boot-load-seg $(LOAD_SEGMENT) -o $(BUILD_DIR)/cornel.iso $(BUILD_DIR)/iso

