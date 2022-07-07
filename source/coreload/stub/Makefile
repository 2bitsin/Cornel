include .config

TARGET_NAME					?=  $(shell basename $(abspath .))
BUILD_DIR						?=	$(abspath ../../../build/$(TARGET_NAME))
SOURCES_DIR					?= 	$(abspath .)
TARGET_BINARY_NAME 	?= 	$(TARGET_NAME).bin
TARGET_BINARY 			?= 	$(BUILD_DIR)/$(TARGET_BINARY_NAME)
MAP_FILE						?= 	$(TARGET_BINARY).map

SOURCES_S						?=	prologue.s		\
				 								longdiv.s			\
				 								biosacpi.s		\
												flatreal.s		\
												epilogue.s		\
												irqstubs.s
												

SOURCES_C						?=	stub.c				\
												smallcode.c		\
				 								error.c				\
				 								memory.c   		\
				 								print.c				\
												rtccmos.c			\
												kbdctrl.c			\
												desctab.c			\
												atwenty.c			\
												serial.c			\
												timer.c

CC									:=	wcc
LD									:=	wlink
AS									:=	wasm

LOAD_ADDRESS				?=	0x7c00
LOAD_SEGMENT				?=	0x7c0
STACK_SIZE					?=	0x1c00
BASE_ADDRESS				?= 	0x6000 # LOAD_ADDRESS - STACK_SIZE
LINKER_FILE					?=	$(SOURCES_DIR)/.wlink

INCLUDE							?=	-i=$(SOURCES_DIR)/include
DEFINES							?=	-dDEBUG=1 -dG_LOAD_ADDRESS=$(LOAD_ADDRESS) -dG_STACK_SIZE=$(STACK_SIZE) -dG_BASE_ADDRESS=$(BASE_ADDRESS) -dDEBUG_STARTUP=1
C_FLAGS							?=	-3 -ms -ox -zu $(DEFINES) $(INCLUDE)
S_FLAGS							?=	-3 -mt $(DEFINES)

OBJECTS							?=	$(SOURCES_S:.s=.o) $(SOURCES_C:.c=.o)

PREFIXED_OBJECTS		?=	$(addprefix $(BUILD_DIR)/, $(OBJECTS))

DEPENDENCY_FILES		?=	$(addprefix $(BUILD_DIR)/, $(SOURCES_C:.c=.d))

.PHONY: all
all: $(BUILD_DIR) $(TARGET_BINARY)

$(BUILD_DIR): 
	mkdir -p $@

.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)/*

$(BUILD_DIR)/%.d : $(SOURCES_DIR)/%.c
	$(CC) $(C_FLAGS) -ad=$@ -adfs -adt=$(@F:.d=.o) $< -fo=$(@:.d=.o)

$(BUILD_DIR)/%.o : $(SOURCES_DIR)/%.c

$(BUILD_DIR)/%.o : $(SOURCES_DIR)/%.c $(BUILD_DIR)/%.d
	$(CC) $(C_FLAGS) -fo=$@ $<

$(BUILD_DIR)/%.o : $(SOURCES_DIR)/%.s
	$(AS) $(S_FLAGS) -fo=$@ $<

$(TARGET_BINARY) : $(PREFIXED_OBJECTS) $(LINKER_FILE)
	$(LD) NAME $(TARGET_BINARY) PATH $(BUILD_DIR) FILE {$(OBJECTS)} @$(LINKER_FILE) OPTION MAP=$(MAP_FILE),OFFSET=$(STACK_SIZE)

include $(wildcard $(BUILD_DIR)/*.d)
				