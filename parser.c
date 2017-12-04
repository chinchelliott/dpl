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
int functionDefinitionPending(Parser *p);
int functionCallPending(Parser *p);
int exprPending(Parser *p);
int primitivePending(Parser *p);
int loopPending(Parser *p);
int ifStatementPending(Parser *p);

Lexeme *parse(FILE *fp) {
  // allocate space for parser struct
  Parser *p = malloc(sizeof(Parser));
  // Initialize values
  p->fp = fp;
  p->pending = lex(p->fp);
  p->line = 1;
  p->out = stdout;
  Lexeme *tree = NULL;


  // return parse tree for evaluation
  //return tree;

  // function parse()
  //       {
  //       // Start symbol is expression, terminator is SEMI
  //
  //       advance();
  //       expression();
  //       match(SEMI);
  //       }
  // Consume Lexemes

  if(statementsPending(p)) {
    tree = statements(p);
  }

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
    fprintf(stderr,"Expected type %s, found %s at line %d\n",t,displayLexeme(p->pending),p->line);
    fprintf(stderr,"Previous type %s\n",display(p->previous));
    exit(1);
  }
}

int typeEqual(struct lexeme *x,struct lexeme *y) {
    return strcmp(x->type,y->type) == 0;
    return 0;
}

// void displayTree(struct lexeme *,char *) {
//     //NOT YET IMPLEMENTED
//     return;
// }


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
    check(p, COMMENT) || check(p, ERROR) || check(p, PRINT);
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
//--------RECOGNIZING FUNCTIONS--------//


// statements:     statement
//                 | statement statements
Lexeme *statements(Parser *p) {
    Lexeme *a,b = NULL;
    a = statement(p);
    if (statementsPending(p)) {
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

    if (exprPending(p)) {
        a = expr(p);
        match(p,SEMI);
    }
    else if (loopPending(p)) return loop(p);
    else if (ifStatementPending(p)) return ifStatement(p);
    else if (utilPending(p)) return utilStatement(p);

    return a;
}

// expr: primary
//     | primary operator expr
//     | keyword:BALLOUT expr (return statement)
Lexeme *expr(Parser *p) {
    Lexeme *a, *b, *c;
    if (primaryPending(p)) {
        a = primary(p);
        if(operatorPending(p)) {
            b = operator(p);
            c = expression(p);
            b->left = a;
            b->right = c;
            return b;
        }
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
    if (varDefPending(p)) return varDefintion(p);
    else if (functionDefPending(p)) return functionDefinition(p);
    else return arrDef(p);
}


// varDefintion:   keyword:MAJORKEY IDENTIFIER ASSIGN expr  //majorkey = var
//                 | keyword:MAJORKEY IDENTIFIER
Lexeme *varDefinition(Parser *p) {
    Lexeme *a = match(p,VAR);
    a->left = match(p,ID);
    if (assignmentPending(p)) {
        advance(p);
        a->right = expr(p);
    }
    else {
        a->right = lexeme(NIL);
    }
    return a;
}


// functionDefinition: keyword:RICKROLL IDENTIFIER OPAREN optParams CPAREN block //rickroll = function
Lexeme *functionDefinition(Parser *p) {
    Lexeme *a = match(p,FUNCTION);
    if (check(ID)) }
        a->left = match(p, ID);
    }
    advance(p);
    Lexeme *b = optParams(p);
    advance(p);
    Lexeme *c = block(p);
    a->right = cons(GLUE, b, c);
    return a;
}

// optParams:  *EMPTY*
//             | params
Lexeme *optParams(Parser *p) {
    Lexeme *a = NULL;
    if (check(ID)) {

    }


}

// functionCall: IDENTIFIER OPAREN optParams CPAREN
//               | keyword:lambduh lambdaCall
Lexeme *functionCall(Parser *p) {
    if (check(IDENTIFIER)) {
        advance(p);
        match(p,OPAREN);
        optParams(p);
        match(p,CPAREN);
    }
    // else {
    //     match(LAMBDA);
    //     lambdaCall(p);
    // }
}


// loop:           keyword:STAYWOKE OPAREN expr CPAREN block		//while loop
Lexeme *loop(Parser *p) {
    Lexeme *a = match(p,WHILE);
    Lexeme *b = match(p,OPAREN);
    a->left = expr(p);
    b = match(p,CPAREN);
    a->right = block(p);

    //match(WHILE);
    // else {
    //     match(FOR);
    //     match(ID);
    //     match(OPAREN);
    //     match
    // }
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
        c = lexeme(NIL);
    }
    a-> right = cons(IFLIST, b, c);

    return a;
}

Lexeme *elseStatement(Parser *p) {
  return block(p);
}

// comment: keyword:ME_IRL: STRING SEMI
Lexeme *comment(Parser *p) {
    match(p,COMMENT);
    match(p,STRING);
    match(p,SEMI);
}

Lexeme *block(Parser *p) {
    if (statementsPending(p)) return statements(p);
    return lexeme(NIL);
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
    if (check(INTEGER)) return match(p,INTEGER);
    else if (check(STRING)) return match(p,STRING);
    //may need to have array function though
    else if (check(ARRAY)) return match(p,ARRAY);
    else if (check(T)) return match(p,T);
    else if (check(F)) return match(p,F);
    else if (check(BAKED)) return match(p,BAKED);
    else if (check(ID)) return match(p,ID);
    //as well as lambda function
    else if (check(LAMBDA)) return match(p,LAMBDA);



    {
        return p->current;
    }
    else match(p,NULL);
}
