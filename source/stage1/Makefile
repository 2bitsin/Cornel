
CC?=wcc
LD?=wlink
AS?=wasm

BUILD_DIR?=../../build/stage1

LOAD_ADDRESS?=0x8000
LOAD_SEGMENT?=0x800
STACK_SIZE?=0x2000

DEFINES?=-dDEBUG=1 -dG_LOAD_ADDRESS=$(LOAD_ADDRESS) -dG_STACK_SIZE=$(STACK_SIZE)
C_FLAGS?=-3 -ms -ox -zu $(DEFINES)
S_FLAGS?=-3 -mt $(DEFINES)

SOURCES?=	startup.s			\
					longdiv.s			\
					biosacpi.s		\
					smallcode.s		\
					stackseg.s		\
					bootstrap.c		\
					error.c				\
					memory.c    	\
					print.c				

$(BUILD_DIR)/%.o : %.c
	$(CC) $(C_FLAGS) -fo=$(BUILD_DIR)/$(@F) $(SOURCES)/$(@F)

$(BUILD_DIR)/%.o : %.s
	$(AS) $(S_FLAGS) -fo=$(BUILD_DIR)/$(@F) $(SOURCES)/$(@F)



				

