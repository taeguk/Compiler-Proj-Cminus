SRC_DIR = .
TARGET = util
LEX_TARGET= tiny.l

build: build.lex build.core
	gcc -c main.c -o $(OBJS_DIR)/main.o
	gcc -o $(SRC_DIR)/../$(MAIN_PROG) $(OBJS_DIR)/*.o

build.lex: tiny.l .mkdir.o
	flex --noyywrap --outfile=$(BUILD_DIR)/lex.c $(LEX_TARGET)
	gcc -c $(BUILD_DIR)/lex.c -o $(OBJS_DIR)/lex.o

build.core: $(addsuffix .o, $(TARGET))


include $(SRC_DIR)/rules.mk
