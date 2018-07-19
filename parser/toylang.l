%{
#include <string>
#include "compile/node.h"
#include "parser.h"
#define SAVE_TOKEN yylval.string = new std::string(yytext, yyleng)
#define TOKEN(t) (yylval.token = t)

%}

%option noyywrap

%x COMMENT
%%
\/\/ { BEGIN (COMMENT); }
<COMMENT>\n   { BEGIN (INITIAL); }
[ \t\n] {}
break  { printf("keyword: %s\n", yytext); return BREAK;}
bool   { printf("keyword: %s\n", yytext); return BOOL;}
continue {printf("keyword: %s\n", yytext); return CONTINUE;}
else   { printf("keyword: %s\n", yytext); return ELSE;}
elsif  { printf("keyword: %s\n", yytext); return ELSIF;}
false  { printf("keyword: %s\n", yytext); return FALSE;}
fn     { printf("keyword: %s\n", yytext); return FUNC;}
for    { printf("keyword: %s\n", yytext); return FOR; }
if     { printf("keyword: %s\n", yytext); return IF; }
lambda { printf("keyword: %s\n", yytext); return LAMBDA;}
match  { printf("keyword: %s\n", yytext); return MATCH;}
return { printf("keyword: %s\n", yytext); return RETURN;}
string { printf("keyword: %s\n", yytext); return STRING;}
struct { printf("keyword: %s\n", yytext); return STRUCT;}
true   { printf("keyword: %s\n", yytext); return TRUE;}
use    { printf("keyword: %s\n", yytext); return USE;}
while  { printf("keyword: %s\n", yytext); return WHILE;}
[a-zA-Z_][a-zA-Z0-9_]*      {printf("identifier: %s\n", yytext); SAVE_TOKEN; return IDENTIFIER;}
[-+]?[0-9]+\.[0-9]*          {printf("double: %s\n", yytext); SAVE_TOKEN; return DOUBLE;}
[-+]?[0-9]+                 {printf("int: %s\n", yytext); SAVE_TOKEN; return INTEGER;}



"(" { printf("token: ( %s\n", yytext); return LP; }
")" { printf("token: ) %s\n", yytext); return RP; }
"{" { printf("token: { %s\n", yytext); return LC; }
"}" { printf("token: } %s\n", yytext); return RC; }
"[" { printf("token: [ %s\n", yytext); return LB; }
"]" { printf("token: ] %s\n", yytext); return RB; }

"="  { return ASSIGN;}
">"  { return GT; }
">=" { return GE; }
"<=" { return LE; }
"<"  { return LT; }

"==" { printf("op: %s\n", yytext); return EQ; }
"!=" { printf("op: %s\n", yytext); return NE; }
"+"  { printf("op: %s\n", yytext); return TOKEN(ADD); }
"-"  { printf("op: %s\n", yytext); return MIS; }
"*"  { printf("op: %s\n", yytext); return TOKEN(MUL); }
"/"  { printf("op: %s\n", yytext); return DIV; }
"%"  { printf("op: %s\n", yytext); return REM; }
">>" { printf("op: %s\n", yytext); return SHR; }
"<<" { printf("op: %s\n", yytext); return SHL; }

"&&" { return LAND; }
"||" { return LOR;  }
"&"  { return BAND; }
"^"  { return BEOR; }
"|"  { return BOR; }

"->" { return RARROW; }
"=>" { return FAT_RARROW; }
":"  { return COLON; }
","  { printf("keyword: %s\n", yytext); return COMMA; }

"+=" {return ADDEQ; }
"-=" {return MISEQ; }
"*=" {return MULEQ; }
"/=" {return DIVEQ; }
"%=" {return REMEQ; }
"&=" { return BANDEQ; }
"^=" { return BEOREQ; }
"|=" { return BOREQ; }
">>=" { return SHREQ; }
"<<=" { return SHLEQ; }



%%