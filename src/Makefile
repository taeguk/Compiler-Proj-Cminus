SRC_DIR = .
TARGET = util
LEX_TARGET= cm.l

BISON_Y=cm.y
BISON_Y_C=cm.c
BISON_Y_H=cm.h

build: build.bison build.lex build.core
	gcc -c main.c -o $(OBJS_DIR)/main.o
	gcc -c $(BUILD_DIR)/lex.c -o $(OBJS_DIR)/lex.o
	gcc -c $(BUILD_DIR)/$(BISON_Y_C) -o $(OBJS_DIR)/$(BISON_Y_C).o
	gcc -o $(SRC_DIR)/../$(MAIN_PROG) $(OBJS_DIR)/*.o

build.lex: cm.l .mkdir.o
	flex --noyywrap --outfile=$(BUILD_DIR)/lex.c $(LEX_TARGET)

build.core: $(addsuffix .o, $(TARGET))

build.bison: build.lex $(BISON_Y)
	bison -o $(BUILD_DIR)/$(BISON_Y_C) -d $(BISON_Y)


include $(SRC_DIR)/rules.mk
