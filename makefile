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

error1:
	cat error1.txt

error1x:
	./slang error1.txt

error2:
	cat error2.txt

error2x:
	./slang error2.txt

error3:
	cat error3.txt

error3x:
	./slang error3.txt

error4:
	cat error4.txt

error4x:
	./slang error4.txt

error5:
	cat error5.txt

error5x:
	./slang error5.txt
