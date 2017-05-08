CC=gcc
CFLAGS=-c -Wall -O3
EXEC=20141589

.SUFFIXES: .c .o

LEXCODE=cm.l
LEXSRC=lex.yy.c
SRCS=main.c util.c $(LEXSRC)
OBJS=$(SRCS:.c=.o)

$(EXEC): $(LEXSRC) $(OBJS)
	$(CC) -o $@ $(OBJS)

$(LEXSRC): $(LEXCODE)
	flex -o $(LEXSRC) $(LEXCODE)

clean:
	rm -f $(OBJS) $(EXEC) $(LEXSRC)
