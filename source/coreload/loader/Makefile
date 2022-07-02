include .config

TARGET_NAME					?=  $(shell basename $(abspath .))
BUILD_DIR						?=	$(abspath ../../../build/coreload/$(TARGET_NAME))
SOURCES_DIR					?= 	$(abspath .)
TARGET_BINARY_NAME 	?= 	$(TARGET_NAME).bin
TARGET_BINARY 			?= 	$(BUILD_DIR)/$(TARGET_BINARY_NAME)
MAP_FILE						?= 	$(TARGET_BINARY).map

CXX_FLAGS						?=	-ffreestanding -m32 -std=c++20 -O3
LD_FLAGS						?= 	-m elf_i386 -T $(SOURCES_DIR)/.ld -Map=$(MAP_FILE)

CXX_SOURCES					?=	$(wildcard *.cpp)
OBJECTS							?= 	$(CXX_SOURCES:.cpp=.o)
PREFIXED_OBJECTS		?=	$(addprefix $(BUILD_DIR)/,$(OBJECTS))


.PHONY: debug
debug:
	@echo OBJECTS=$(OBJECTS)
	@echo PREFIXED_OBJECTS=$(PREFIXED_OBJECTS)

.PHONY: all
all: $(BUILD_DIR) $(TARGET_BINARY) 

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)/*

$(BUILD_DIR)/%.d : $(SOURCES_DIR)/%.cpp
	$(CXX) -MM -MG $(CXX_FLAGS) -c $< -o $@

$(BUILD_DIR)/%.o : $(SOURCES_DIR)/%.cpp

$(BUILD_DIR)/%.o : $(SOURCES_DIR)/%.cpp $(BUILD_DIR)/%.d
	$(CXX) $(CXX_FLAGS) -c $< -o $@

$(TARGET_BINARY): $(PREFIXED_OBJECTS)
	$(LD) $(LD_FLAGS) -o $(TARGET_BINARY) $(PREFIXED_OBJECTS)

include $(wildcard $(SOURCES_DIR)/*.d)
	
