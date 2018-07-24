%{

%}

%union{



}

%token
%token
%token


%type


%start program

%%
program : stmt_list { }
    ;

stmt_list : stmt { }
    | stmt_list stmt { }
    ;

stmt : var_decl | func_decl | extern_decl
    |

var_decl : ident  ident {}
    | ident ident EQ expr {}
    ;






%%