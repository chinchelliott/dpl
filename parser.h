#ifndef SLANG_PARSER
#define SLANG_PARSER
#include "lexer.h"

typedef struct parser {
  FILE *fp;
  struct lexeme *previous;
  struct lexeme *pending;
  FILE *out;
  int line;
} Parser;

struct lexeme *parse(FILE *in);

void displayTree(struct lexeme *,char *);
int typeEqual(struct lexeme *,struct lexeme *);

#endif
