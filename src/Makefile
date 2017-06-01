SRC_DIR = .
BUILD_DIR = $(SRC_DIR)/build
OBJS_DIR = $(BUILD_DIR)/objs

LEX_CODE= cm.l
LEX_SRC=cm.lex.c

BISON_CODE=cm.y
BISON_SRC=cm.tab.c
BISON_HDR=cm.tab.h

CC_FLAGS = 

TARGET = util analyze symtab

# Targets
build: build.bison build.lex build.core
	gcc -o $(SRC_DIR)/../$(MAIN_PROG) $(OBJS_DIR)/*.o

build.lex: cm.l .mkdir.o
	flex --noyywrap --outfile=$(BUILD_DIR)/$(LEX_SRC) $(LEX_CODE)

build.core: $(addsuffix .o, $(TARGET))
	gcc -c $(CC_FLAGS) $(BUILD_DIR)/$(LEX_SRC) \
		-o $(OBJS_DIR)/$(basename $(LEX_SRC)).o
	gcc -c $(CC_FLAGS) $(BUILD_DIR)/$(BISON_SRC) \
		-o $(OBJS_DIR)/$(basename $(BISON_SRC)).o
	gcc -c $(CC_FLAGS) main.c -o $(OBJS_DIR)/main.o

build.bison: build.lex $(BISON_CODE)
	bison -o $(BUILD_DIR)/$(BISON_SRC) -vd $(BISON_CODE)


# make warn : add gcc warning options
.PHONY: warn
warn: CC_FLAGS+= -Wall
warn: CC_FLAGS+= -pedantic
# warn: CC_FLAGS+= -Wtraditional
warn: CC_FLAGS+= -Wshadow
warn: CC_FLAGS+= -Wpointer-arith
warn: CC_FLAGS+= -Wcast-qual
warn: CC_FLAGS+= -Wcast-align
warn: CC_FLAGS+= -Wstrict-prototypes
warn: CC_FLAGS+= -Wmissing-prototypes
warn: CC_FLAGS+= -Wconversion
warn: build


# make debug: add -g option, and we can debug.
.PHONY: debug
debug: CC_FLAGS += -g -D DEBUG
debug: build

include $(SRC_DIR)/rules.mk
