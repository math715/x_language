%{

%}

%union{

}

%token
%token
%token


%type


%start top

%%
top : stmts { }
    ;

stmts : stmt { }
    | stmts stmt { }
    ;

stmt : var_decl | func_decl | extern_decl




%%