%{
#include <string>
#include "compile/node.h"
#include "parser.h"
#define SAVE_TOKEN yylval.string = new std::string(yytext, yyleng)
#define TOKEN(t) (yylval.token = t)

extern "C" int yywrap(){}
%}

%x COMMENT
%%
<INITIAL>"\/\/" { BEGIN (COMMENT); }
<COMMENT>"\n"   { BEGIN (INITIAL); }
<INITIAL>[ \t\n] {}
<INITIAL>[a-zA-Z_][a-zA-Z0-9_]* {SAVE_TOKEN; return IDENTIFIER;}
<INITIAL>[-+]?[0-9]+.[0-9]*          {SAVE_TOKEN; return DOUBLE;}
<INITIAL>[-+]?[0-9]+                 {SAVE_TOKEN; return INTEGER;}
<INITIAL>"break"  {  return  BREAK;}
<INITIAL>"bool"   { return BOOL;}
<INITIAL>"continue" { return CONTINUE;}
<INITIAL>"else"   { return ELSE;}
<INITIAL>"elsif"  { return ELSIF;}
<INITIAL>"false"  { return FALSE;}
<INITIAL>"fn"     { return FUNC;}
<INITIAL>"for"    { return FOR; }
<INITIAL>"if"     { return IF; }
<INITIAL>"lambda" { return LAMBDA;}
<INITIAL>"match"  { return MATCH;}
<INITIAL>"return" { return RETURN;}
<INITIAL>"string" { return STRING;}
<INITIAL>"struct" { return STRUCT;}
<INITIAL>"true"   { return TRUE;}
<INITIAL>"use"    { return USE;}
<INITIAL>"while"  { return WHILE;}


<INITIAL>"(" { return LP; }
<INITIAL>")" { return RP; }
<INITIAL>"{" { return LC; }
<INITIAL>"}" { return RC; }
<INITIAL>"[" { return LB; }
<INITIAL>"]" { return RB; }

<INITIAL>"=" { return ASSIGN;}
<INITIAL>">"  { return GT; }
<INITIAL>">=" { return GE; }
<INITIAL>"<=" { return LE; }
<INITIAL>"<"  { return LT; }

<INITIAL>"==" { return EQ; }
<INITIAL>"!=" { return NE; }
<INITIAL>"+"  { return ADD; }
<INITIAL>"-"  { return MIS; }
<INITIAL>"*"  { return MUL; }
<INITIAL>"/"  { return DIV; }
<INITIAL>"%"  { return REM; }
<INITIAL>">>" { return SHR; }
<INITIAL>"<<" { return SHL; }

<INITIAL>"&&" { return LAND; }
<INITIAL>"||" { return LOR;  }
<INITIAL>"&"  { return BAND; }
<INITIAL>"^"  { return BEOR; }
<INITIAL>"|"  { return BOR; }

<INITIAL>"->" { return RARROW; }
<INITIAL>"=>" { return FAT_RARROW; }
<INITIAL>":"  { return COLON; }
<INITIAL>","  { return COMMA; }

<INITIAL>"+=" {return ADDEQ; }
<INITIAL>"-=" {return MISEQ; }
<INITIAL>"*=" {return MULEQ; }
<INITIAL>"/=" {return DIVEQ; }
<INITIAL>"%=" {return REMEQ; }
<INITIAL>"&=" { return BANDEQ; }
<INITIAL>"^=" { return BEOREQ; }
<INITIAL>"|=" { return BOREQ; }
<INITIAL>">>=" { return SHREQ; }
<INITIAL>"<<=" { return SHLEQ; }



%%