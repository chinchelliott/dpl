OBJS = slanguage.o lexer.o types.o parser.o
OOPTS = -Wall -Wextra -g -std=c99 -c
LOPTS = -Wall -Wextra -g -std=c99

all: slang

slang: $(OBJS)
	gcc $(LOPTS) -o slang $(OBJS)

slanguage.o: slanguage.c lexer.h parser.h
	gcc $(OOPTS) slanguage.c

lexer.o: lexer.c lexer.h types.h
	gcc $(OOPTS) lexer.c

parser.o: parser.c parser.h types.h
	gcc $(OOPTS) parser.c

types.o: types.c types.h
	gcc $(OOPTS) types.c

clean:
	rm -f $(OBJS) slang

test:
	./slang testfile.slang
