CC=gcc
CFLAGS=-c -Wall -O3
EXEC=20141589

.SUFFIXES: .c .o

LEXCODE=cm.l
LEXSRC=cm.lex.c

BISONCODE=cm.y
BISONSRC=cm.tab.c
BISONHDR=cm.tab.h
BISONVERBOSE=cm.output

SRCS=main.c util.c $(LEXSRC) $(BISONSRC)
OBJS=$(SRCS:.c=.o)

$(EXEC): $(LEXSRC) $(BISONSRC) $(OBJS)
	$(CC) -o $@ $(OBJS)

$(BISONSRC): $(LEXSRC) $(BISONCODE)
	bison -o $(BISONSRC) -vd $(BISONCODE)

$(LEXSRC): $(LEXCODE)
	flex -o $(LEXSRC) $(LEXCODE)

clean:
	rm -f $(OBJS) $(EXEC) $(LEXSRC) $(BISONSRC) $(BISONHDR) $(BISONVERBOSE)
