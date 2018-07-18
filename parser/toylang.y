%{
    #include "compile/node.h"
    BlockNode *program_block;
    extern int yylex();
    void yyerror(const char *s) {
        printf("Error: %s\n", s);
    }
%}

%union{
    Node *node;
    BlockNode  *block;
    Expression *expr;
    Statement  *stmt;
    IdentifierNode *ident;
    VariableDeclaration *var_decl;
    std::vector<VariableDeclaration *> *var_vec;
    std::vector<Expression*> *expr_vec;
    std::string *string;
    int token;
}

%token <string> IDENTIFIER INTEGER DOUBLE BOOLEAN
%token <token>  LC RC BREAK EXTERN ASSIGN
%token <token>  COLON COMMA FUNC
%token <token>  ADD MIS MUL DIV REM
%token <token>  ADDEQ MISEQ MULEQ DIVEQ REMEQ BANDEQ BEOREQ BOREQ SHREQ SHLEQ
%token <token>  FALSE TRUE
%token <token>  GT GE LE LT EQ NE
%token <token>  LR RR LP RP LB RB
%token <token>  RARROW FAT_RARROW
%token <token>  SHL SHR
%token <token>  BOOL
%token <token> CONTINUE ELSE ELSIF FOR IF LAMBDA MATCH RETURN USE WHILE
%token <token> STRING STRUCT
%token <token> LAND LOR BAND BEOR BOR

%type <ident> ident
%type <expr> scalar expr
%type <var_vec> func_decl_args
%type <expr_vec> call_args
%type <block> program stmts block
%type <stmt> stmt var_decl func_decl extern_decl
%type <token> comparison opterion_eq

%start program

%%
program : stmts { program_block = $1; }
        ;

stmts : stmt { $$ = new BlockNode(); $$->statements_.push_back($<stmt>1); }
        | stmts stmt {$1->statements_.push_back($<stmt>2);}
        ;

stmt : var_decl  | func_decl | extern_decl
        | expr { $$ = new ExpressionStatement(*$1); }
        ;


block : LC stmts RC { $$ = $2 ; }
        | LP RP { $$ = new BlockNode(); }
        ;

var_decl : ident ident { $$ = new VariableDeclaration(*$1, *$2); }
        | ident ident ASSIGN expr { $$ = new VariableDeclaration(*$1, *$2, $4); }
        ;

extern_decl : EXTERN ident ident LC func_decl_args RC
            { $$ = new ExternDecleration(*$2, *$3, *$5); delete $5;}
            ;

/*func fn_name ( args... ) -> type */
func_decl : FUNC ident LC func_decl_args RC RARROW ident block
            { $$ = new FunctionDeclaration(*$7, *$2, *$4, *$8); delete $4; }
            ;

func_decl_args : /*blank*/ { $$ = new VariableList(); }
            | var_decl { $$ = new VariableList(); $$->push_back($<var_decl>1); }
            | func_decl_args COMMA var_decl { $1->push_back($<var_decl>1); }
            ;


ident : IDENTIFIER { $$ = new IdentifierNode(*$1); delete $1; }
            ;


scalar : INTEGER { $$ = new IntegerNode( atol($1->c_str())); delete $1; }
        | DOUBLE { $$ = new DoubleNode( atof($1->c_str())); delete $1; }
        | FALSE { $$ = new BooleanNode(false); }
        | TRUE { $$ = new BooleanNode ( true); }
        ;

expr : ident EQ expr { $$ = new AssignmentNode(*$<ident>1, *$3); }
        | ident LP call_args RP { $$ = new FunctionCallNode(*$1, *$3); delete $3; }
        | ident { $<ident>$ = $1 ; }
        | scalar
        | expr MUL expr { $$ = new BinaryOperatorNode(*$1, $2, *$3); }
        | expr DIV expr { $$ = new BinaryOperatorNode(*$1, $2, *$3); }
        | expr REM expr { $$ = new BinaryOperatorNode(*$1, $2, *$3); }
        | expr ADD expr { $$ = new BinaryOperatorNode(*$1, $2, *$3); }
        | expr MIS expr { $$ = new BinaryOperatorNode(*$1, $2, *$3); }
        | expr SHR expr { $$ = new BinaryOperatorNode(*$1, $2, *$3); }
        | expr SHL expr { $$ = new BinaryOperatorNode(*$1, $2, *$3); }
        | expr comparison expr { $$ = new BinaryOperatorNode(*$1, $2, *$3); }
        | expr opterion_eq expr { $$ = new BinaryOperatorNode(*$1, $2, *$3); }
        | LP expr RP { $$ = $2 ; }
        ;




call_args : /*blank*/ { $$ = new ExpressionList(); }
        | expr { $$ = new ExpressionList(); $$->push_back($1); }
        | call_args COMMA expr { $$->push_back($3); }
        ;

comparison : GT | GE | LE | LT | EQ | NE

opterion_eq: ADDEQ | MISEQ | MULEQ | DIVEQ |  REMEQ | BANDEQ | BEOREQ | BOREQ | SHREQ |  SHLEQ

%%