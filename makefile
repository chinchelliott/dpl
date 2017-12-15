OBJS = slanguage.o lexer.o types.o parser.o env.o eval.o
OOPTS = -Wall -Wextra -g -std=c99 -c
LOPTS = -Wall -Wextra -g -std=c99

all: dpl

dpl: $(OBJS)
	gcc $(LOPTS) -o dpl $(OBJS)

slanguage.o: slanguage.c lexer.h parser.h env.h types.h eval.h
	gcc $(OOPTS) slanguage.c

lexer.o: lexer.c lexer.h types.h
	gcc $(OOPTS) lexer.c

parser.o: parser.c parser.h types.h
	gcc $(OOPTS) parser.c

types.o: types.c types.h
	gcc $(OOPTS) types.c

env.o: env.c env.h types.h parser.h lexer.h
	gcc $(OOPTS) env.c

eval.o: eval.c eval.h env.h lexer.h types.h
	gcc $(OOPTS) eval.c

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

arrays:
	cat arrays.slang

arraysx:
	./dpl arrays.slang

conditionals:
	cat conditionals.slang

conditionalsx:
	./dpl conditionals.slang

recursion:
	cat recursion.slang

recursionx:
	./dpl recursion.slang

lambda:
	cat lambda.slang

lambdax:
	./dpl lambda.slang

problem:
	cat testfile.slang

problemx:
	./dpl testfile.slang

functions:
	cat functions.slang

functionsx:
	./dpl functions.slang

