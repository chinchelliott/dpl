OBJS = slanguage.o lexer.o types.o parser.o
OOPTS = -Wall -Wextra -g -std=c99 -c
LOPTS = -Wall -Wextra -g -std=c99

all: dpl

dpl: $(OBJS)
	gcc $(LOPTS) -o dpl $(OBJS)

slanguage.o: slanguage.c lexer.h parser.h
	gcc $(OOPTS) slanguage.c

lexer.o: lexer.c lexer.h types.h
	gcc $(OOPTS) lexer.c

parser.o: parser.c parser.h types.h
	gcc $(OOPTS) parser.c

types.o: types.c types.h
	gcc $(OOPTS) types.c

clean:
	rm -f $(OBJS) dpl

test:
	./dpl testfile.slang

error1:
	cat error1.slang

error1x:
	./dpl error1.slang

error2:
	cat error2.slang

error2x:
	./dpl error2.slang

error3:
	cat error3.slang

error3x:
	./dpl error3.slang

error4:
	cat error4.slang

error4x:
	./dpl error4.slang

error5:
	cat error5.slang

error5x:
	./dpl error5.slang
