CC = gcc
CFLAGS = -std=gnu99

OBJS = cminus.tab.o main.o util.o lex.yy.o
TARGET = 20141500

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) -lfl

main.o: main.c globals.h util.h scan.h 
	$(CC) $(CFLAGS) -c main.c

util.o: util.c util.h globals.h 
	$(CC) $(CFLAGS) -c util.c

lex.yy.o: lex.yy.c util.h globals.h scan.h 
	$(CC) $(CFLAGS) -c lex.yy.c

lex.yy.c: lex/cminus.l
	flex lex/cminus.l

cminus.tab.o: cminus.tab.c cminus.tab.h
	$(CC) $(CFLAGS) -c cminus.tab.c

cminus.tab.c cminus.tab.h: yacc/cminus.y
	bison -d yacc/cminus.y

clean:
	rm -rf $(TARGET) $(OBJS)

all: $(TARGET)
