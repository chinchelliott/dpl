#include "parser.h"
#include "types.h"

Lexeme *parse(FILE *fp);
int check(Parser *p, char *t);
void advance(Parser *p);
Lexeme *match(Parser *p, char *t);
void matchNoAdvance(Parser *p, char *t);
char *getType(Lexeme *l);
int typeEqual(struct lexeme *x,struct lexeme *y);

//pending functions
int statementsPending(Parser *p);
int statementPending(Parser *p);
int varDefPending(Parser *p);
int functionDefPending(Parser *p);
int functionCallPending(Parser *p);
int exprPending(Parser *p);
int primitivePending(Parser *p);
int loopPending(Parser *p);
int ifStatementPending(Parser *p);
int primaryPending(Parser *p);
int defExprPending(Parser *p);
int idExprPending(Parser *p);
int arrDefPending(Parser *p);
int booleanOpPending(Parser *p);
int utilPending(Parser *p);



//lexeme functions
Lexeme *statements(Parser *p);
Lexeme *statement(Parser *p);
Lexeme *expr(Parser *p);
Lexeme *primary(Parser *p);
Lexeme *defExpr(Parser *p);
Lexeme *varDefinition(Parser *p);
Lexeme *functionDefinition(Parser *p);
Lexeme *functionCall(Parser *p);
Lexeme *optParams(Parser *p);
Lexeme *optArgs(Parser *p);
Lexeme *loop(Parser *p);
Lexeme *ifStatement(Parser *p);
Lexeme *elseStatement(Parser *p);
Lexeme *comment(Parser *p);
Lexeme *block(Parser *p);
Lexeme *primitive(Parser *p);
Lexeme *utilStatement(Parser *p);
Lexeme *operator(Parser *p);
Lexeme *booleanOp(Parser *p);
Lexeme *idExpr(Parser *p);
Lexeme *lambdaBody(Parser *p);
Lexeme *arrDefinition(Parser *p);
Lexeme *printStatement(Parser *p);

Lexeme *parse(FILE *fp) {
  // allocate space for parser struct
  Parser *p = malloc(sizeof(Parser));
  // Initialize values
  p->fp = fp;
  p->pending = lex(p->fp);
  p->line = 1;
  p->out = stdout;
  Lexeme *tree = NULL;

  if(statementsPending(p)) {
    tree = statements(p);
  }

  return tree;

}

//--------LEXICAL HELPER FUNCTIONS--------//

int check(Parser *p, char *t) {
    return getType(p->pending) == t;
}

char *getType(Lexeme *l) {
    return l->type;
}

void advance(Parser *p) {
    p->previous = p->pending;
    p->pending = lex(p->fp);
    if (check(p,NEWLINE)) {
        advance(p);
        p->line +=1;
    }
}

Lexeme *match(Parser *p, char *t) {
  Lexeme *current = p->pending;
  matchNoAdvance(p,t);
  advance(p);
  return current;
}

void matchNoAdvance(Parser *p, char *t) {

  if(!check(p,t)) {
    printf("illegal\n");
    fprintf(stderr,"Syntax error\n");
    fprintf(stderr,"Expected type %s, found %s at line %d\n",t,displayLexeme(*(p->pending)),p->line);
    fprintf(stderr,"Previous type %s\n",displayLexeme(*(p->previous)));
    exit(1);
  }
}

int typeEqual(struct lexeme *x,struct lexeme *y) {
    return strcmp(x->type,y->type) == 0;
    return 0;
}

void displayTree(Lexeme *tree, char *head) {
    if(tree != NULL) {
        char *newHead = malloc(sizeof(char) * 1024);
        strcat(newHead,head);
        strcat(newHead,"\t");
        displayTree(tree->left,newHead);
        printf("%s%s\n",head,displayLexeme(*tree));
        displayTree(tree->right,newHead);
    }
}


//--------PENDING FUNCTIONS--------//

int statementsPending(Parser *p) {
    return statementPending(p);
}

int statementPending(Parser *p) {
    return exprPending(p) || loopPending(p) || ifStatementPending(p)  || utilPending(p);
}

int exprPending(Parser *p) {
    return primaryPending(p) || check(p,RETURN);
}

int primaryPending(Parser *p) {
    return primitivePending(p) || defExprPending(p) || idExprPending(p) || check(p,LAMBDA);
}

int defExprPending(Parser *p) {
    return varDefPending(p) || functionDefPending(p) || arrDefPending(p);
}

int varDefPending(Parser *p) {
    return check(p, VAR);
}

int functionDefPending(Parser *p) {
    return check(p, FUNCTION);
}

int arrDefPending(Parser *p) {
    return check(p, ARRAY);
}

int idExprPending(Parser *p) {
    return check(p, ID) || functionCallPending(p);
}

int functionCallPending(Parser *p) {
    return check(p, ID);
}

int loopPending(Parser *p) {
    return check(p, WHILE);
}

int ifStatementPending(Parser *p) {
    return check(p,IF);
}

int utilPending(Parser *p) {
    return check(p, COMMENT) || check(p, ERROR) || check(p, PRINT);
}

int primitivePending(Parser *p) {
    return check(p, INTEGER) || check(p, STRING) || check(p, NIL) || check(p, T) || check(p, F);
}

int assignmentPending(Parser *p) {
    return check(p,ASSIGN);
}

int operatorPending(Parser *p) {
    return check(p, PLUS) || check(p, MINUS) || check(p, TIMES) || check(p, DIVIDE) || booleanOpPending(p);
}

int booleanOpPending(Parser *p) {
    return check(p, EQ) || check(p, NEQ) || check(p, NEG) || check(p, LESS_THAN) || check(p, GREATER_THAN) || check(p, OR) || check(p, AND);
}

int elsePending(Parser *p) {
    return check(p, ELSE);
}

int paramsPending(Parser *p) {
    return check(p, ID);
}

int commaPending(Parser *p) {
    return check(p, COMMA);
}
//--------RECOGNIZING FUNCTIONS--------//


// statements:     statement
//                 | statement statements
Lexeme *statements(Parser *p) {
    Lexeme *a,*b = lexeme(NIL); //CHANGED FROM NULL
    //NOTE make sure to never cons NULL with something, must be lexeme(NIL)
    printf("processed first statement\n");
    a = statement(p);
    if (statementsPending(p)) {
        printf("processing more statements\n");
        b = statements(p);
    }
    return cons(STATEMENTLIST,a,b);
}

// statement:      expr SEMI
//                 | loop
//                 | ifStatement
//           TODO      | utilStatement
Lexeme *statement(Parser *p) {
    Lexeme *a = NULL;

    // if (exprPending(p)) {
    //     a = expr(p);
    //     match(p,SEMI);
    //     return a; //might need to change this back? not sure
    // }
    if (loopPending(p)) return loop(p);
    else if (ifStatementPending(p)) return ifStatement(p);
    else if (utilPending(p)) return utilStatement(p);
    else {
        a = expr(p);
        printf("this is it 1\n");
        match(p,SEMI);
    }

    return a;
}

// expr: primary SEMI (just added semi, so expr always has semi)
//     | primary operator expr
//     | keyword:BALLOUT expr (return statement)
Lexeme *expr(Parser *p) {
    Lexeme *a, *b, *c = NULL;
    if (primaryPending(p)) {
        a = primary(p);
        printf("primary found %s ", displayLexeme(*a));
        if(operatorPending(p)) {
            printf("operation pending\n");
            b = operator(p);
            c = expr(p);
            b->left = a;
            b->right = c;
            return b;
        }
        //match(p,SEMI); //or advance? not sure
        return a;
    }
    else {
        match(p, RETURN);
        return expr(p);
    }
}

// primary: primitive
//         | defExpr //aka, defining variables or functions variable usage
//         | idExpr //using variables or calling functions
//         | keyword:lambduh lambdaBody //could be defining or calling
Lexeme *primary(Parser *p) {
    printf("in primary\n");
    if (primitivePending(p)) return primitive(p);
    else if (defExprPending(p)) return defExpr(p);
    else if (idExprPending(p)) return idExpr(p);
    else {
        match(p,LAMBDA);
        return lambdaBody(p);
    }
}

// defExpr: varDefintion
//         | functionDefinition
//         | arrDef
Lexeme *defExpr(Parser *p) {
    if (varDefPending(p)) return varDefinition(p);
    else if (functionDefPending(p)) return functionDefinition(p);
    else return arrDefinition(p);
}


// varDefintion:   keyword:MAJORKEY IDENTIFIER ASSIGN expr  //majorkey = var
//                 | keyword:MAJORKEY IDENTIFIER
Lexeme *varDefinition(Parser *p) {
    printf("in variable definition");
    Lexeme *a = match(p,VAR);
    a->left = match(p,ID);
    if (assignmentPending(p)) {
        advance(p);
        a->right = expr(p);
    }
    // else {
    //     a->right = lexeme(NIL);
    // } SHOULDNT NEED THIS
    return a;
}


// functionDefinition: keyword:RICKROLL IDENTIFIER OPAREN optParams CPAREN block //rickroll = function
Lexeme *functionDefinition(Parser *p) {
    Lexeme *a = match(p,FUNCTION);
    //if (check(p, ID)) {
        a->left = match(p, ID);
    //}
    match(p,OPAREN);
    Lexeme *b = NULL; //changed from LEXEME(NIL)
    if (paramsPending(p)) {
        printf("params pending\n");
        b = optParams(p);
    }
    match(p,CPAREN);
    printf("out of that\n");
    //advance(p);
    printf("confused\n");
    Lexeme *c = block(p);
    printf("confused\n");
    //TODO function's lambda here
    a->right = cons(LAMBDA, b, c);
    return a;
}

// optParams:  *EMPTY*
//             | params
Lexeme *optParams(Parser *p) {
    Lexeme *a = match(p, ID);
    a->left = NULL; //changed from lexeme(NIL)
    a->right = NULL; //changed from lexeme(NIL)
    if (commaPending(p)) {
        a->right = optParams(p);
    }
    return a;
}

// functionCall: IDENTIFIER OPAREN optArgs CPAREN
//               | keyword:lambduh lambdaCall
//TODO: not using this right now, functioncall handled within idexpr and lambdabody
Lexeme *functionCall(Parser *p) {
    Lexeme *a = match(p, ID);
    Lexeme *b = NULL; //changed from lexeme(NIL)
    //advance(p);
    match(p,OPAREN);
    if (exprPending(p)) {
        b = optArgs(p);
    }
    //advance(p);
    match(p, CPAREN);
    return cons(FUNCALL, b, a);
}

Lexeme *optArgs(Parser *p) {
    Lexeme *a = expr(p);
    a->left = NULL; //changed from lexeme(NIL)
    a->right = NULL; //changed from lexeme(NIL)
    if (check(p, COMMA)) {
        a->right = optArgs(p);
    }
    return a;
}


// loop:           keyword:STAYWOKE OPAREN expr CPAREN block		//while loop
Lexeme *loop(Parser *p) {
    Lexeme *a = match(p,WHILE);
    match(p, OPAREN);
    a->left = expr(p);
    match(p, CPAREN);
    a->right = block(p);
    return a;
}

// ifStatement: keyword:FORREAL OPAREN expr CPAREN block optElse
Lexeme *ifStatement(Parser *p) {
    Lexeme *temp;
    Lexeme *a = match(p,IF);
    temp = match(p,OPAREN);
    a->left = expr(p);
    temp = match(p,CPAREN);
    Lexeme *b,*c = NULL;
    b = block(p);
    if (elsePending(p)) {
        c = elseStatement(p);
    }
    else {
        c = NULL; //changed from lexeme(NIL)
    }
    a-> right = cons(IFLIST, b, c);

    return a;
}

Lexeme *elseStatement(Parser *p) {
    Lexeme *a = match(p,ELSE);
    a->left = block(p);
    a->right = NULL; //changed from lexeme(NIL)
    return a;
}

Lexeme *block(Parser *p) {
    Lexeme *a = NULL;
    match(p,OBRACE);
    if (statementsPending(p)) {
        a = statements(p);
    }
    match(p,CBRACE);
    return a; //changed from lexeme(NIL)
}

Lexeme *utilStatement(Parser *p) {
    if (check(p, COMMENT)) return comment(p);
    else {
        return printStatement(p);
    }
    // else return match(p, NULL);
}

Lexeme *operator(Parser *p) {
    //TODO
    if (check(p, PLUS)) return match(p, PLUS);
    else if (check(p, MINUS)) return match(p, MINUS);
    else if (check(p, TIMES)) return match(p, TIMES);
    else if (check(p, DIVIDE)) return match(p, DIVIDE);
    else {
        return booleanOp(p);
    }
}

Lexeme *booleanOp(Parser *p) {
    if (check(p,EQ)) return match(p, EQ);
    else if (check(p, NEQ)) return match(p, NEQ);
    else if (check(p, NEG)) return match(p, NEG);
    else if (check(p, LESS_THAN)) return match(p, LESS_THAN);
    else if (check(p, GREATER_THAN)) return match(p, GREATER_THAN);
    else if (check(p, OR)) return match(p, OR);
    else return match(p, AND);
}

// idExpr: IDENTIFIER
//         | IDENTIFIER ASSIGN expr
//         | functionCall
//         | IDENTIFIER OBRACKET expr CBRACKET //going into arrays?
Lexeme *idExpr(Parser *p) {
    Lexeme *a = match(p, ID);
    printf("in id , %s\n",displayLexeme(*a));
    Lexeme *b = NULL; //changed from null
    //redefinition of variable
    if (check(p,ASSIGN)) {
        match(p,ASSIGN);
        printf("redef pending\n");
        b = expr(p);
        printf("redefinition of a variable called %s to %s\n", displayLexeme(*a), displayLexeme(*b));
        return cons(VAR, a, b);
    }
    //indexing into array, or array call
    if (check(p,OBRACKET)) {
        match(p,OBRACKET);
        b = expr(p);
        match(p,CBRACKET);
        return cons(ARRAYCALL, a, b);
    }
    //function call
    if (check(p,OPAREN)) {
        match(p,OPAREN);
        if (exprPending(p)) {
            b = optArgs(p);
        }
        match(p, CPAREN);
        return cons(FUNCALL, b, a);
    }
    return a;
}

//lambdaBody: OPAREN optParams CPAREN block lambdaCall
Lexeme *lambdaBody(Parser *p) {
    //TODO
    Lexeme *a = match(p, LAMBDA);
    Lexeme *b = NULL;
    //Lexeme *c = lexeme(FUNCTION);
    //c->left = lexeme(NIL);

    match(p, OPAREN);
    if (paramsPending(p)) {
        a->left = optParams(p);
    }
    match(p, CPAREN);
    a->right = block(p);

    if (check(p,OPAREN)) {
        match(p,OPAREN);
        b = optArgs(p);
        match(p, CPAREN);
        return cons(FUNCALL, b, a);
    }

    return a;
}

// arrDef:| keyword:FAM IDENTIFIER OPARR optArgs CLARR //array, with <> denoting contents
//         | FAM size oparen int cparen //need size function?
Lexeme *arrDefinition(Parser *p) {
    //TODO need size constructor for arrays
    Lexeme *a = match(p, ARRAY);
    a->left = match(p, ID);
    match(p,OPARR);
    match(p,CLARR);
    //Lexeme *c = NULL;
    if (exprPending(p)) {
        a->right = optArgs(p);
    }
    return a;
}

Lexeme *printStatement(Parser *p) {
    Lexeme *a = match(p,PRINT);
    match(p, STRING);
    match(p, SEMI);
    return a;
}

// comment: keyword:ME_IRL: STRING SEMI
Lexeme *comment(Parser *p) {
    Lexeme *a = match(p,COMMENT);
    // advance(p);
    // advance(p);
    // match(p,STRING);
    while (!check(p,SEMI)) {
        advance(p);
    }
    match(p,SEMI);
    return a;
}

// primitive: keyword:SUMN //integer
//            | keyword:SWAG //string
//            | keyword:BAKED //null
// 	       | keyword:FAM //array, with <> denoting contents
//            | keyword:TRUUU
//            | keyword:FAKENEWS
//            | IDENTIFIER
//            | keyword:LAMBDUH lambdaBody
Lexeme *primitive(Parser *p) {
    // printf("in primitive expression\n");
    if (check(p, INTEGER)) return match(p,INTEGER);
    else if (check(p, STRING)) return match(p,STRING);
    //TODO may need to have array function though
    //else if (check(ARRAY)) return match(p,ARRAY);
    else if (check(p, T)) return match(p,T);
    else if (check(p, F)) return match(p,F);
    else if (check(p, NIL)) return match(p,NIL);
    else if (check(p, ID)) return match(p,ID);
    //TODO as well as lambda function
    //else if (check(LAMBDA)) return match(p,LAMBDA);
    // {
    //     return p->current;
    // }
    else return match(p,NULL);
}
