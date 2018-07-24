%{

%}

%%

"fn" {return FUNC;}
"if" {return IF;}
"else" {return ELSE;}
"return" {return RETURN;};
"extern" {return EXTERN;}
"for" {return FOR;}


[A-Za-z][A-Za-z0-9]*      {return IDENTIFIER;}
([-+]?[1-9][0-9]*) | "0"  {return NUMBER; }

"+" return ADD;
"-" return SUB;
"*" return MUL;
"/" return DIV;
"%" return REM;

"(" return LP;
")" return RP;
"{" return LC;
"}" return RC;



