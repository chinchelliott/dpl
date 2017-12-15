#include "lexer.h"
#include "parser.h"
#include "env.h"
#include "types.h"
#include "eval.h"

int main(int argc,char **argv) {


    if (argc == 1) {
        fprintf(stdout,"Number of commands: %d command, no test file specified, exiting program\n",argc);
        exit(-1);
    }

   FILE *fp = fopen(argv[1], "r");
    
//    FILE *fp = fopen("testfile.slang", "r");


    Lexeme *myTree = parse(fp);
//
    displayTree(myTree,"");
    
	Lexeme *global = createEnv();
    eval(myTree, global);
    displayEnv(global);


//
//    printf("Creating a new environment\n");
//    Lexeme *global = createEnv();
//    printf("The envronment is:\n");
//    displayEnv(global);
//    Lexeme *x = lexeme(ID);
//    x->sval = "x";
//    Lexeme *val = lexeme(INTEGER);
//    val->ival = 3;
//
//    printf("Inserting variable x with value 3 into environment\n");
//    insertEnv(x,val,global);
//
//    printf("The environment is:\n");
//    displayEnv(global);
//
//    printf("Extending the environment with y:4 and z:\"hello\"\n");
//    Lexeme *y = lexeme(ID);
//    Lexeme *z = lexeme(ID);
//    y->sval = "y";
//    z->sval = "z";
//    Lexeme *yval = lexeme(INTEGER);
//    yval->ival = 4;
//    Lexeme *zval = lexeme(STRING);
//    zval->sval = "hello";
//
//    Lexeme *local = createEnv();
//    insertEnv(y,yval,local);
//    insertEnv(z,zval,local);
//    Lexeme *table = car(local);
//
//    local = extendEnv(car(table),cdr(table),global);
//    printf("The local environment is:\n");
//    displayEnv(local);
//    printf("The environment is:\n");
//    displayEnv(global);

return 0;
}

