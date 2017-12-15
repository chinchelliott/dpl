#ifndef SLANG_LEXER
#define SLANG_LEXER
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

// typedef struct parser Parser;

typedef struct lexeme {
  char *type;
  char *sval;
  int ival;
  double rval;

  struct lexeme *left;
  struct lexeme *right;

  // struct lexeme **aval;

  // struct lexeme *(*builtin)(struct lexeme *,struct lexeme *);
} Lexeme;

Lexeme *lex(FILE *);
Lexeme *lexeme(char *);
Lexeme *cons(char *,Lexeme *, Lexeme *);
Lexeme *car(Lexeme *);
Lexeme *cdr(Lexeme *);
 void setCar(Lexeme *, Lexeme *);
 void setCdr(Lexeme *, Lexeme *);
char *displayLexeme(Lexeme);
// char *display(Lexeme *);

#endif
