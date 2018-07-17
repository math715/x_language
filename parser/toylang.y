%{
    #include "compiler/node.h"
    NBlock *program_block;
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
%token <token>  LC RC
%token <token>
%token <token>

%type <ident> ident
%type <expr> numeric expr
%type <var_vec> func_decl_args
%type <expr_vec> call_args
%type <block> program stmts block
%type <stmt> stmt var_decl func_decl extern_decl
%type <token> comparison

%start program

%%
program : stmts { programBlock = $1; }
        ;

stmts : stmt { $$ = new BlockNode();; $$->statements.push_back($<stmt>1); }
        | stmts stmt {$1->statements.push_back($<stmt>2);}
        ;

stmt : var_decl  | func_decl
        | expr { $$ = new ExpressionStatement(*$1); }
        ;

block : LC stmts RC { $$ = $2 ; }
        | LC RC { $$ = new BlockNode(); }
        ;

var_del : ident ident { $$ = new VariableDeclaration(*$1, $2); }
        | ident COLON type EQ

%%