#include "lexer.h"
#include "parser.h"

int main(int argc,char **argv) {


    if (argc == 1) {
        fprintf(stdout,"Number of commands: %d command, exiting program\n",argc);
        exit(-1);
    }

    FILE *fp = fopen(argv[1], "r");

    // Lexeme *mylex;
    //
    // mylex = lex(fp);
    //
    // while (strcmp(mylex->type,"END_OF_FILE")) {
    //     display(mylex);
    //     printf("\n");
    //     mylex = lex(fp);
    // }

    Lexeme *myTree = parse(fp);



return 0;
}
