#include "lexer.h"
#include "parser.h"

int main(int argc,char **argv) {


    // if (argc == 1) {
    //     fprintf(stdout,"Number of commands: %d command, exiting program\n",argc);
    //     exit(-1);
    // }

    FILE *fp = fopen("testfile.slang", "r");

    // Lexeme *mylex;
    //
    // mylex = lex(fp);
    //
    // while (strcmp(mylex->type,"END_OF_FILE")) {
    //     display(mylex);
    //     printf("\n");
    //     mylex = lex(fp);
    // }

    printf("Beginning parsing\n");
    Lexeme *myTree = parse(fp);
    printf("Parsing completed!\n");

    displayTree(myTree,"");

return 0;
}
