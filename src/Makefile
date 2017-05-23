SRC_DIR = .
BUILD_DIR = $(SRC_DIR)/build
OBJS_DIR = $(BUILD_DIR)/objs

LEX_CODE= cm.l
LEX_SRC=cm.lex.c

BISON_CODE=cm.y
BISON_SRC=cm.tab.c
BISON_HDR=cm.tab.h


TARGET = util

build: build.bison build.lex build.core # $(BUILD_DIR)/$(BISON_HDR)
	gcc -o $(SRC_DIR)/../$(MAIN_PROG) $(OBJS_DIR)/*.o

build.lex: cm.l .mkdir.o
	flex --noyywrap --outfile=$(BUILD_DIR)/$(LEX_SRC) $(LEX_CODE)

build.core: $(addsuffix .o, $(TARGET))
	gcc -c $(BUILD_DIR)/$(LEX_SRC) \
		-o $(OBJS_DIR)/$(basename $(LEX_SRC)).o
	gcc -c $(BUILD_DIR)/$(BISON_SRC) \
		-o $(OBJS_DIR)/$(basename $(BISON_SRC)).o
	gcc -c main.c -o $(OBJS_DIR)/main.o

build.bison: build.lex $(BISON_CODE)
	bison -o $(BUILD_DIR)/$(BISON_SRC) -vd $(BISON_CODE)


include $(SRC_DIR)/rules.mk
