
CC=wcc
LD=wlink
AS=wasm

BUILD_DIR						?=	../../build/stage1
SOURCES_DIR					?= 	.

LOAD_ADDRESS				?=	0x8000
LOAD_SEGMENT				?=	0x800
STACK_SIZE					?=	0x2000
TARGET_BINARY_NAME 	?= 	stage1.bin
TARGET_BINARY 			?= 	$(BUILD_DIR)/$(TARGET_BINARY_NAME)

DEFINES							?=	-dDEBUG=1 -dG_LOAD_ADDRESS=$(LOAD_ADDRESS) -dG_STACK_SIZE=$(STACK_SIZE)
C_FLAGS							?=	-3 -ms -ox -zu $(DEFINES) -i=$(SOURCES_DIR)/include
S_FLAGS							?=	-3 -mt $(DEFINES)
INCLUDE							?=	-i=include

SOURCES_S						?=	startup.s			\
				 								longdiv.s			\
				 								biosacpi.s		\
				 								smallcode.s		\
				 								stackseg.s		

SOURCES_C						?=	bootstrap.c		\
				 								error.c				\
				 								memory.c   		\
				 								print.c

OBJECTS							?=	$(SOURCES_S:.s=.o) $(SOURCES_C:.c=.o)

PREFIXED_OBJECTS		?=	$(addprefix $(BUILD_DIR)/, $(OBJECTS))

DEPENDENCY_FILES		?=	$(addprefix $(BUILD_DIR)/, $(SOURCES_C:.c=.d))

.PHONY: all
all: $(TARGET_BINARY)

.PHONY: clean
clean: ; rm -rf $(BUILD_DIR)/*
	
$(BUILD_DIR)/%.o : $(SOURCES_DIR)/%.c

$(BUILD_DIR)/%.o : $(SOURCES_DIR)/%.c $(BUILD_DIR)/%.d
	$(CC) $(C_FLAGS) -fo=$@ $<

$(BUILD_DIR)/%.d : $(SOURCES_DIR)/%.c
	$(CC) $(DEFINES) $(INCLUDE) -ad=$< -adfs -adt=$(@F:.d=.o) $< 

$(BUILD_DIR)/%.o : $(SOURCES_DIR)/%.s
	$(AS) $(S_FLAGS) -fo=$@ $<

$(TARGET_BINARY) : $(PREFIXED_OBJECTS) $(SOURCES_DIR)/.wlink
	$(LD) NAME $(TARGET_BINARY) PATH $(BUILD_DIR) FILE {$(OBJECTS)} OPTION MAP=$(BUILD_DIR)/$(TARGET_BINARY_NAME).map @$(SOURCES_DIR)/.wlink

include $(wildcard $(DEPENDENCY_FILES))
				