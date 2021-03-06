#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lexer.h"
#include "types.h"
#include <ctype.h>
//#include "parser.h"

/*Recall that each symbol to the left of the colon is called a non-terminal.
Each non-terminal will correspond to a function.
The right hand side of the rule, which the non-terminal heads, will correspond to the body of the function.
Non-terminals on the right hand side correspond to a call to the function associated with that non-terminal
*/

Lexeme *lex(FILE *fp);
Lexeme *lexeme(char *s);
Lexeme *badLexeme(char c);
Lexeme *lexVar(char c,FILE *fp);
Lexeme *lexNumber(char c, FILE *fp);
Lexeme *lexString(char c, FILE *fp);
void display(Lexeme *l);
void skipWhiteSpace(FILE *fp);
int isWhiteSpace(char c);
char getNextCharacter(FILE *fp);
char *resize(char *original, int *size);

Lexeme *lex(FILE *fp) {

        //make input an instance of a scanner
        //scanner Input =
        //pushback needs to be ungetc

        skipWhiteSpace(fp);

        char ch = getNextCharacter(fp);

        //indicating that c is end of input
        //if (Input.failed) return new Lexeme(END);

        if(ch==EOF) {
            return lexeme(END_OF_FILE);
        }

        if (ch=='\n') {
            return lexeme(NEWLINE);
        }
//        else {
//            skipWhiteSpace(fp);
//        }


//TODO: Ive commented out all the ones i dont think i need...
        switch(ch)
            {
            // single character tokens
            case '(':
                return lexeme(OPAREN);
            case ')':
                return lexeme(CPAREN);
            case ',':
                return lexeme(COMMA);
            case ';':
                return lexeme(SEMI);
            case '=':
                return lexeme(ASSIGN);
            case '{':
                return lexeme(OBRACE);
            case '}':
                return lexeme(CBRACE);
            case '<':
                return lexeme(OPARR);
            case '>':
                return lexeme(CLARR);
            case '[':
                return lexeme(OBRACKET);
            case ']':
                return lexeme(CBRACKET);
            case '+':
                return lexeme(PLUS);
            case '-':
                return lexeme(MINUS);
            case '*':
                return lexeme(TIMES);
            case '/':
                return lexeme(DIVIDE);
            case '%':
                return lexeme(MODULO);
            case ':':
                return lexeme(COLON);

            default:
                // multi-character tokens
                // (only numbers, variables/keywords, and strings)
                if (isdigit(ch))
                    {
                    //ungetc(ch,fp);
                    return lexNumber(ch, fp);
                    }
                else if (isalpha(ch))
                    {
                    //ungetc(ch,fp);
                    return lexVar(ch, fp);  //and keywords!
                    }
                else if (ch == '\"')
                    {
                    //ungetc(ch,fp);
                    return lexString(ch, fp);
                    }
                else {
                    return badLexeme(ch);
                }
            }
}

//you pass in the type and its created, update values later
Lexeme *lexeme(char *s) {
  Lexeme *new = malloc(sizeof(Lexeme));
  new->type = s;
  new->sval = NULL;
  new->ival = 0;
  new->rval = 0;
  new->left = NULL;
  new->right = NULL;
  //new->builtin = NULL;
  // printf("new lexeme of type %s\n", new->type);
  return new;
}

//----------TYPE FUNCTIONS----------//


Lexeme *badLexeme(char c) {
  Lexeme *new = lexeme(BAD_LEX);
  char *bad_char = malloc(sizeof(char));
  bad_char[0] = c;
//  printf("bad lexeme found: %s", bad_char);
  new->sval = bad_char;
  return new;
}

Lexeme *lexVar(char c,FILE *fp) {

//    printf("new var char character is %c\n",c);
    int full = 1; int size = 20;
    char current;

    Lexeme *new = lexeme(ID);
    char *name = malloc(sizeof(char) * size);
    name[0] = c;

    current = getNextCharacter(fp);
//    printf("THIS IS THE NEXT CHARACTER %c !!!\n",current);

    while(!isWhiteSpace(current) && (isalpha(current) || isdigit(current)) &&  current != EOF && current != '\n') {
        if(full >= size) {
          name = resize(name,&size);
        }
        name[full++] = current;
        current = getNextCharacter(fp);
    }

//    printf("new var is %s\n", name);

      ungetc(current,fp);
      name[full++] = '\0';
      if(!strcmp(name,"rickroll")) {
        new->type = FUNCTION;
      }
      else if(!strcmp(name,"majorkey")) {
        new->type = VAR;
      }
      else if(!strcmp(name,"ballout")) {
        new->type = RETURN;
      }
      else if(!strcmp(name,"smh")) {
        new->type = ERROR;
      }
      else if(!strcmp(name,"throwshade")) {
        new->type = PRINT;
      }
      else if(!strcmp(name,"staywoke")) {
        new->type = WHILE;
      }
      else if(!strcmp(name,"baked")) {
        new->type = NIL;
      }
      else if(!strcmp(name,"lambduh")) {
        new->type = LAMBDA;
      }
      else if(!strcmp(name,"lit")) {
        new->type = FOR;
      }
      else if(!strcmp(name,"forreal")) {
        new->type = IF;
      }
      else if(!strcmp(name,"ornah")) {
        new->type = ELSE;
      }
      else if(!strcmp(name,"thicc")) {
        new->type = PLUS;
      }
      else if(!strcmp(name,"zonked")) {
        new->type = MINUS;
      }
      else if(!strcmp(name,"af")) {
        new->type = TIMES;
      }
      else if(!strcmp(name,"dab")) {
        new->type = DIVIDE;
      }
      else if(!strcmp(name,"legit")) {
        new->type = EQ;
      }
      else if(!strcmp(name,"harambe")) {
        new->type = NEQ;
      }
      else if(!strcmp(name,"nah")) {
        new->type = NEG;
      }
      else if(!strcmp(name,"thirsty")) {
        new->type = LESS_THAN;
      }
      else if(!strcmp(name,"hella")) {
        new->type = GREATER_THAN;
      }
      else if(!strcmp(name,"chill")) {
        new->type = OR;
      }
      else if(!strcmp(name,"highkey")) {
        new->type = AND;
      }
      else if(!strcmp(name,"fam")) {
        new->type = ARRAY;
      }
      else if(!strcmp(name,"lusth")) {
        new->type = T;
      }
      else if(!strcmp(name,"fakenews")) {
        new->type = F;
      }
      else if(!strcmp(name,"MEIRL")) {
        new->type = COMMENT;
      }
      else {
        new->sval = name;
      }



      return new;


}


Lexeme *lexNumber(char c, FILE *fp) {
  int filled = 1, size = 16;
  char n;
  char *num = malloc(sizeof(char) * size);
  num[0] = c;
  n = getNextCharacter(fp);
    
  while(isdigit(n) && !isWhiteSpace(n) && n != EOF) {
    
    if(filled >= size) {
      num = resize(num,&size);
    }
    num[filled++] = n;
    n = getNextCharacter(fp);
  }
  ungetc(n,fp);
  num[filled++] = '\0';
    Lexeme *new;

//    if (dflag == 1) {
//        new = lexeme(REAL);
//        new->rval = atof(num);
//        new->sval = num;
//    }
//    else {
	new = lexeme(INTEGER);
 	 new->ival = atoi(num);
  	new->sval = num;
//    }

  return new;
}


Lexeme *lexString(char c, FILE *fp) {
  int filled = 0, size = 16;
  char n;
  Lexeme *new = lexeme(STRING);
  char *string = malloc(sizeof(char) * size);
  string[0] = c;

  n = getNextCharacter(fp);

  while(n != '\"') {
    if(n == EOF) {
      fprintf(stderr,"FATAL ERROR: STRING NOT CLOSED\n");
      exit(1);
    }
    if(filled >= size) {
      string = resize(string,&size);
    }
    if(n != '\\') {
      string[filled++] = n;
    } else {
      n = getNextCharacter(fp);
      if(n == 'n') {
        string[filled++] = '\n';
      } else if(n == 't') {
        string[filled++] = '\t';
      } else if(n == 'r') {
        string[filled++] = '\r';
      } else {
        string[filled++] = n;
      }
    }
    n = getNextCharacter(fp);
  }
  string[filled++] = '\0';
  new->sval = string;
  new->ival = filled;

  return new;
}



//-------DISPLAY FUNCTION-------//

// void display(Lexeme *l) {
    //char *displayLexeme(Lexeme l) {
    //   char *string = malloc(sizeof(char) * 64);
      // if(l->type == INTEGER) {
      //   printf("%s %d",l->type,l->ival);
      // }
      // else {
      //   printf("%s %s",l->type, l->sval);
      // }
  // }
char *displayLexeme(Lexeme l) {
    char *string = malloc(sizeof(char) * 100);
    if(l.type == INTEGER) {
        sprintf(string,"%s %d",l.type,l.ival);
    }
    else if(l.type == STRING || l.type == ID || l.type == BAD_LEX) {
        sprintf(string,"%s %s",l.type,l.sval);
    }
    else {
        //printf("attempting to print\n");
        sprintf(string,"%s",l.type);
    }
    return string;
}



Lexeme *cons(char *type, Lexeme *left, Lexeme *right) {
  Lexeme *new = lexeme(type);
  new->left = left;
  new->right = right;

  return new;
}

Lexeme *car(Lexeme *l) {
  return l->left;
}

Lexeme *cdr(Lexeme *l) {
  return l->right;
}

void setCar(Lexeme *l, Lexeme *left) {
    l->left = left;
}

void setCdr(Lexeme *l, Lexeme *right) {
    l->right = right;
}




//------- SCANNING FUNCTIONS -------//
// void skipWhiteSpace(Parser *p) {
void skipWhiteSpace(FILE *fp) {
    char c = getNextCharacter(fp);

    while(!feof(fp) && isWhiteSpace(c)) {
        // if(c == '\n') {
        //     p->line++;
        // }
        c = getNextCharacter(fp);
    }
    ungetc(c,fp);
}

int isWhiteSpace(char c) {
    return c == ' ' || c == '\r' || c == '\t';
}

//int getNextCharacter(Parser *p) {
char getNextCharacter(FILE *fp) {
    //int c = fgetc(p->fp);
    int result;
    char x;
    result = fgetc(fp);
    x = result;
//    result = fscanf(fp,"%c",&x);
    if (result == EOF)
        {
        return EOF;
        }
    if (result == 0)
        {
        fprintf(stderr,"SCAN ERROR: attempt to read a raw character failed\n");
        fprintf(stderr,"offending character was <%c>\n",fgetc(fp));
        exit(2);
        }
    return x;

    //TODO fix comments, ignoring comments
    // if(c == '/') {
    //     int s = fgetc(p->fp);
    //     if(s == '/') {
    //         while(!feof(p->fp) && c != '\n') {
    //             c = fgetc(p->fp);
    //         }
    //     } else {
    //         ungetc(s,p->fp);
    //     }
    // }

    //return c;
}

char *resize(char *original, int *size) {
  *size *= 2;
  return realloc(original, sizeof(char) * (*size));
}
