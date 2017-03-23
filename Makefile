SRC_DIR = .
TARGET = util lex.yy
LEX_TARGET= tiny.l
MAIN_PROG = main

build: build.core build.lex
	gcc -c main.c -o $(BUILD_DIR)/main.o
	gcc -o $(SRC_DIR)/$(MAIN_PROG) $(BUILD_DIR)/*.o

build.lex: .mkdir.o
	flex --noyywrap $(LEX_TARGET)

build.core: $(addsuffix .o, $(TARGET))


include $(SRC_DIR)/rules.mk
