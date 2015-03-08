%{
#include <cstdio>
#include <iostream>
using namespace std;
#define YY_DECL extern "C" int yylex()

#include "dragoon.tab.h"  //for tokens
int yycolumn = 1;
/* called before the code in each of our rules
   we use it to update bison on position info, since bison expects us to set
   yyloc now that we've given it the %locations directive.  yycolumn is reset to
   1 upon each newline */
#define YY_USER_ACTION yylloc.first_line = yylloc.last_line = yylineno;\
    yylloc.first_column = yycolumn; yylloc.last_column = yycolumn + yyleng - 1;\
    yycolumn += yyleng;

#define TOKEN(t) (yylval.token = t)

%}
/* noyywrap so that it doesn't call yywrap upon EOF- we do that for it.  noinput
   and nounput because we aren't using those functions, causing warnings */
%option noyywrap noinput nounput

/* enable line number tracking */
%option yylineno

%%

"module"          return TOKEN(MODULE);
"if"              return TOKEN(IF);
[(){}.,;]         return *yytext;

"="               return TOKEN(EQUAL);
"+"               return TOKEN(PLUS);
"-"               return TOKEN(MINUS);
"*"               return TOKEN(MUL);
"/"               return TOKEN(DIV);

[ \t]             ;
\n                { yycolumn = 1; }
[0-9]+\.[0-9]+    { yylval.fval = atof(yytext); return VFLOAT; }
[0-9]+            { yylval.ival = atoi(yytext); return VINT; }
[a-zA-Z0-9]+      {
  yylval.sval = strdup(yytext);
  return VSTRING;
}
.                 {
  yylval.sval = strdup(yytext);
  return BADTOK;
}
%%
